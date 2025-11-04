#include "chroma/server/Server.h"
#include "GameObject_generated.h"
#include "chroma/shared/packet/InputMessage.h"
#include "chroma/shared/packet/PacketHandler.h"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <enet.h>
#include <flatbuffers/buffer.h>
#include <flatbuffers/flatbuffer_builder.h>
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
  peer_last_processed_input_.clear();
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
          world_simulation_.Update(static_cast<float>(elapsed) / 1000.0F);
          peer_last_processed_input_[event.peer] = input_message->GetSeq();
        }

        enet_packet_destroy(event.packet);
        break;
      }
    }

    tick_counter_++;

    if (tick_counter_ >= TICKS) {
      last_tick = now;
      tick_counter_ = 0;
      BroadcastGameObjectState(elapsed);
    }

    world_simulation_.Update(static_cast<float>(elapsed) / 1000.0F);
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

void Server::ConnectClient(const ENetEvent &event)
{
  if (!is_running_ || event.peer == nullptr || event.type != ENET_EVENT_TYPE_CONNECT) { return; }

  auto player = world_simulation_.CreatePlayer();

  connected_players_.emplace(event.peer, player->GetId());
  peer_last_processed_input_.emplace(event.peer, 0);

  flatbuffers::FlatBufferBuilder builder(1024);
  const std::vector<flatbuffers::Offset<Game::EntityState>> game_entities =
    world_simulation_.GetEntitiesFlatBuffer(builder);

  auto game_state =
    chroma::shared::packet::PacketHandler::GameObjectsToFlatBuffer(builder, game_entities, player->GetId(), 0, 0);

  ENetPacket *packet = enet_packet_create(game_state.data(), game_state.size(), ENET_PACKET_FLAG_RELIABLE);

  enet_peer_send(event.peer, 0, packet);
  enet_host_flush(server_.get());
}

void Server::DisconnectClient(const ENetEvent &event)
{
  if (event.type != ENET_EVENT_TYPE_DISCONNECT) { return; }

  if (!is_running_ || !server_) { return; }

  if (server_->connectedPeers == 0) { is_running_ = false; }

  connected_players_.erase(event.peer);
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


void Server::BroadcastGameObjectState(uint64_t delta_time) const
{
  if (!server_) { return; }


  for (const auto &[peer, player_id] : connected_players_) {
    flatbuffers::FlatBufferBuilder builder(1024);
    const std::vector<flatbuffers::Offset<Game::EntityState>> game_entities =
      world_simulation_.GetEntitiesFlatBuffer(builder);

    auto game_state = chroma::shared::packet::PacketHandler::GameObjectsToFlatBuffer(
      builder, game_entities, player_id, delta_time, peer_last_processed_input_.at(peer));

    ENetPacket *packet = enet_packet_create(game_state.data(), game_state.size(), ENET_PACKET_FLAG_RELIABLE);

    enet_peer_send(peer, 0, packet);
  }

  enet_host_flush(server_.get());
}

void Server::Run() { Start(); }

}// namespace chroma::server
