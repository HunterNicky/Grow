// #include "chroma/shared/core/GameObject.h"
#include "chroma/server/Server.h"

#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <enet.h>
#include <memory>
// #include <utility>
// #include <vector>

namespace chroma::server {

Server::Server() : server_(nullptr), address_(), is_running_(false), tick_counter_(0)
{
  if (!InitServer(6969, 2)) {
    is_running_ = false;
    return;
  } else {
    is_running_ = true;
  }
}

Server::Server(ENetHost *server,
               ENetAddress address,
               bool is_running,
               int tick_counter,
               std::vector<std::shared_ptr<shared::core::GameObject>> game_objects)
    : server_(server), address_(address), is_running_(is_running), tick_counter_(tick_counter), game_objects_(std::move(game_objects))
{}

Server::~Server() { Stop(); }

int Server::Start()
{
  if (!is_running_ || server_ == nullptr) { return -1; }

  ENetEvent event;

  auto last_tick = std::chrono::steady_clock::now();

  while (is_running_) {
    while (is_running_ && enet_host_service(server_.get(), &event, 0) > 0) {
      switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT:
          if (ConnectClient(event)) {}
          break;

        case ENET_EVENT_TYPE_RECEIVE:
          enet_packet_destroy(event.packet);
          break;

        case ENET_EVENT_TYPE_DISCONNECT:
          if (DisconnectClient(event)) {}
          break;

        default:
          break;
      }

    }

    tick_counter_++;
  
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
  
    if (tick_counter_ >= TICKS) {
      last_tick = now;
      tick_counter_ = 0;
      BroadcastGameObjectState();
    }
    OnUpdate(static_cast<float>(elapsed) / 1000.0F);
  }

  return 0;
}

int Server::Stop() {
    if (server_) {
        server_.reset();                 
        return 0;
    }
    return -1;
}


bool Server::ConnectClient(const ENetEvent &event) const
{
  if (!IsRunning()) { return false; }

  if (event.peer == nullptr || event.type != ENET_EVENT_TYPE_CONNECT) { return false; }


  // event.peer->data = (void *)"Cliente conectado"; não pode usar conversão de ponteiros no estilo do c
  return true;
}

// NOLINTNEXTLINE
bool Server::DisconnectClient(const ENetEvent &event)
{
    if (event.type != ENET_EVENT_TYPE_DISCONNECT)
    {
      return false;
    }

    if (!is_running_ || server_ == nullptr)
    {
      return false;
    }

    if (server_ != nullptr&& server_->connectedPeers == 0) {
        SetRunning(false);
    }

    return true;
}


bool Server::InitServer(int port, int max_clients)
{
  if (enet_initialize() != 0) { return false; }

  address_.host = ENET_HOST_ANY;
  address_.port = static_cast<enet_uint16>(port);
  server_ = std::shared_ptr<ENetHost>(enet_host_create(&address_, static_cast<size_t>(max_clients), 2, 0, 0),
    [](ENetHost* host){ enet_host_destroy(host); });

  if (server_ == nullptr) {
    enet_deinitialize();
    return false;
  }

  return true;
}

void Server::OnUpdate(float delta_time)
{
  for (auto &game_object : game_objects_) {
    if (game_object->IsActive()) { game_object->OnUpdate(delta_time); }
  }
}

void Server::BroadcastGameObjectState() const
{
  if (server_ == nullptr) { return; }
}

void Server::Run() { Start(); }

}// namespace chroma::server
