#include "chroma/app/states/network/NetworkState.h"
#include "chroma/app/states/State.h"
#include "chroma/app/states/mediator/GameNetworkMediator.h"
#include "chroma/server/Server.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventDispatcher.h"
#include "chroma/shared/events/KeyEvent.h"
#include "chroma/shared/packet/InputMessage.h"
#include "chroma/shared/packet/PacketHandler.h"

#include <chrono>
#include <cstdint>
#include <enet.h>
#include <future>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

namespace chroma::app::states {

void NetworkState::PeerDeleter(ENetPeer *peer)
{
  if (peer != nullptr && peer->host != nullptr) { enet_peer_reset(peer); }
}


NetworkState::NetworkState()
  : State("NetworkState"), client_(nullptr, &enet_host_destroy), server_peer_(nullptr, &PeerDeleter),
    server_address_({}), event_({})
{
  if (!InitNetworkClient()) {
    connected_ = false;
    return;
  }
}

NetworkState::NetworkState(std::shared_ptr<chroma::shared::event::EventDispatcher> event_dispatcher)
  : State("NetworkState"), client_(nullptr, &enet_host_destroy), server_peer_(nullptr, &PeerDeleter),
    server_address_({}), event_({}), event_dispatcher_(std::move(event_dispatcher))
{
  if (!InitNetworkClient()) {
    connected_ = false;
    return;
  }
}

NetworkState::NetworkState(std::shared_ptr<GameNetworkMediator> game_mediator)
  : State("NetworkState"), client_(nullptr, &enet_host_destroy), server_peer_(nullptr, &PeerDeleter),
    server_address_({}), event_({}), game_mediator_(std::move(game_mediator))
{
  if (!InitNetworkClient()) {
    connected_ = false;
    return;
  }
}

bool NetworkState::InitNetworkClient()
{
  if (enet_initialize() != 0) { return false; }

  client_ =
    std::unique_ptr<ENetHost, decltype(&enet_host_destroy)>(enet_host_create(nullptr, 1, 2, 0, 0), &enet_host_destroy);
  if (client_ == nullptr) { return false; }

  if (!ConnectToServer("127.0.0.1", 6969)) {
    connected_ = false;
    return false;
  }
  return true;
}

NetworkState::~NetworkState() { DisconnectFromServer(); }


void NetworkState::DisconnectFromServer()
{
  if (!server_peer_) { return; }

  enet_peer_disconnect(server_peer_.get(), 0);

  bool disconnected = false;
  while (enet_host_service(client_.get(), &event_, 3000) > 0) {
    if (event_.type == ENET_EVENT_TYPE_DISCONNECT && event_.peer == server_peer_.get()) {
      disconnected = true;
      break;
    }
  }

  if (!disconnected && server_peer_) { enet_peer_reset(server_peer_.get()); }

  server_peer_.reset();
  connected_ = false;
}


void NetworkState::OnUpdate(float delta_time)
{

  (void)delta_time;
  while (enet_host_service(client_.get(), &event_, 0) > 0) { ProcessEvent(); }
}

void NetworkState::OnReceiveData() const
{
  if (event_.type == ENET_EVENT_TYPE_RECEIVE) {
    std::span<const uint8_t> packet_span{ static_cast<const uint8_t *>(event_.packet->data),
      event_.packet->dataLength };
    std::vector<uint8_t> packet_data(packet_span.begin(), packet_span.end());

    game_mediator_->OnSnapshotReceived(packet_data);
    enet_packet_destroy(event_.packet);
  }
}

bool NetworkState::TryConnect(const std::string &host, enet_uint16 port)
{
  server_peer_.reset();

  server_address_.port = port;
  enet_address_set_host(&server_address_, host.c_str());

  if (!client_) { return false; }

  server_peer_ = std::unique_ptr<ENetPeer, decltype(&enet_peer_reset)>(
    enet_host_connect(client_.get(), &server_address_, 2, 0), &PeerDeleter);
  enet_host_flush(client_.get());

  return (enet_host_service(client_.get(), &event_, 1000) > 0 && event_.type == ENET_EVENT_TYPE_CONNECT);
}

bool NetworkState::ConnectToServer(const std::string &host, enet_uint16 port)
{
  if (!TryConnect(host, port)) {
    std::promise<bool> ready;
    auto fut = ready.get_future();

    std::thread thread_server([&ready]() {
      auto server = std::make_shared<chroma::server::Server>();
      ready.set_value(server->IsRunning());
      server->Run();
    });
    thread_server.detach();

    if (fut.wait_for(std::chrono::seconds(2)) == std::future_status::ready && fut.get()) {
      if (!TryConnect(host, port)) {
        connected_ = false;
        return false;
      }
    }
  }

  connected_ = true;
  return true;
}


void NetworkState::ProcessEvent()
{
  switch (event_.type) {
  case ENET_EVENT_TYPE_CONNECT:
    break;
  case ENET_EVENT_TYPE_RECEIVE:
    OnReceiveData();
    break;
  case ENET_EVENT_TYPE_DISCONNECT:
    server_peer_ = nullptr;
    break;
  default:
    break;
  }
}

void NetworkState::OnEvent(shared::event::Event &event)
{
  if (!IsActive() || !server_peer_) { return; }

  auto input_message = std::make_shared<chroma::shared::packet::InputMessage>();

  input_message->SetSeq(seq_num_++);
  input_message->SetDeltaTime(0.016F);
  input_message->SetEventType(event.GetType());
  input_message->SetEvent(event);

  auto buf = chroma::shared::packet::PacketHandler::InputMessageToFlatBuffer(input_message);
  if (buf.empty()) { return; }

  ENetPacket *packet = enet_packet_create(buf.data(), buf.size(), ENET_PACKET_FLAG_RELIABLE);
  enet_peer_send(server_peer_.get(), 0, packet);
  enet_host_flush(client_.get());
}

void NetworkState::SetEventDispatcher(const std::shared_ptr<chroma::shared::event::EventDispatcher> &event_dispatcher)
{
  event_dispatcher_ = event_dispatcher;

  event_dispatcher_->Subscribe<shared::event::KeyEvent>([this](shared::event::Event &event) { this->OnEvent(event); });
}

}// namespace chroma::app::states
