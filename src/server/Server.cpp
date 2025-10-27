#include "chroma/server/Server.h"
#include "chroma/shared/packet/InputMessage.h"
#include "chroma/shared/packet/PacketHandler.h"
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <enet.h>
#include <memory>
#include <vector>

namespace chroma::server {

Server::Server()
  : server_(nullptr,
      [](ENetHost *h) {
        if (h) { enet_host_destroy(h); }
      }),
    address_{}, is_running_(false), tick_counter_(0)
{
  if (!InitServer(6969, 2)) { return; }

  is_running_ = true;
}


Server::Server(ENetHost *server, ENetAddress address, bool is_running, int tick_counter)
  : server_(server,
      [](ENetHost *h) {
        if (h) { enet_host_destroy(h); }
      }),
    address_(address), is_running_(is_running), tick_counter_(tick_counter)
{}

Server::~Server()
{
  Stop();

  if (server_) {
    enet_host_destroy(server_.get());
    server_.reset();
  }

  connected_players_.clear();
}

int Server::Start()
{
  if (!is_running_ || !server_) { return -1; }

  ENetEvent event;
  auto last_tick = std::chrono::steady_clock::now();

  while (is_running_) {

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();

    while (is_running_ && enet_host_service(server_.get(), &event, 1) > 0) {
      switch (event.type) {
      case ENET_EVENT_TYPE_CONNECT:
        ConnectClient(event);
        break;
      case ENET_EVENT_TYPE_DISCONNECT:
        DisconnectClient(event);
        break;
      case ENET_EVENT_TYPE_RECEIVE:
        const std::shared_ptr<chroma::shared::packet::InputMessage> input_message =
          shared::packet::PacketHandler::FlatBufferToInputMessage(event.packet->data, event.packet->dataLength);

        if (input_message != nullptr) {
          world_simulation_.OnReceivedInputMessage(input_message, connected_players_[event.peer]);
          world_simulation_.Update(static_cast<float>(elapsed) / 10000.0F);
        }
        enet_packet_destroy(event.packet);
        break;
      }
    }

    tick_counter_++;

    if (tick_counter_ >= TICKS) {
      last_tick = now;
      tick_counter_ = 0;
      BroadcastGameObjectState();
    }

    world_simulation_.Update(static_cast<float>(elapsed) / 10000.0F);
  }

  return 0;
}

int Server::Stop()
{
  if (!server_) { return -1; }

  server_.reset();
  enet_deinitialize();
  is_running_ = false;

  return 0;
}

bool Server::ConnectClient(const ENetEvent &event)
{
  if (!is_running_ || event.peer == nullptr || event.type != ENET_EVENT_TYPE_CONNECT) { return false; }

  auto player = world_simulation_.CreatePlayer();
  connected_players_.emplace(event.peer, player->GetId());

  std::vector<uint8_t> game_state = world_simulation_.GetGameStateSnapshot(player->GetId());

  ENetPacket *packet = enet_packet_create(game_state.data(), game_state.size(), ENET_PACKET_FLAG_RELIABLE);

  enet_peer_send(event.peer, 0, packet);
  enet_host_flush(server_.get());

  return true;
}


bool Server::DisconnectClient(const ENetEvent &event)
{
  if (event.type != ENET_EVENT_TYPE_DISCONNECT) { return false; }

  if (!is_running_ || !server_) { return false; }

  if (server_->connectedPeers == 0) { is_running_ = false; }

  connected_players_.erase(event.peer);

  return true;
}

bool Server::InitServer(int port, int max_clients)
{
  if (enet_initialize() != 0) { return false; }

  address_.host = ENET_HOST_ANY;
  address_.port = static_cast<enet_uint16>(port);

  ENetHost *host = enet_host_create(&address_, static_cast<size_t>(max_clients), 2, 0, 0);
  if (host == nullptr) {
    enet_deinitialize();
    return false;
  }

  server_.reset(host, [](ENetHost *h) {
    if (h) { enet_host_destroy(h); }
  });

  return true;
}


void Server::BroadcastGameObjectState() const
{
  if (!server_) { return; }
  std::vector<uint8_t> game_state = world_simulation_.GetGameStateSnapshot(UUIDv4::UUID{});

  for (const auto &[peer, player_id] : connected_players_) {
    ENetPacket *packet = enet_packet_create(game_state.data(), game_state.size(), ENET_PACKET_FLAG_RELIABLE);

    enet_peer_send(peer, 0, packet);
  }

  enet_host_flush(server_.get());
}

void Server::Run() { Start(); }

}// namespace chroma::server
