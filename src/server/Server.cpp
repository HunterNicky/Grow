#include "chroma/server/Server.h"
#include "chroma/shared/packet/PacketHandler.h"
#include <chrono>

namespace chroma::server {

Server::Server()
  : server_(nullptr, [](ENetHost* h){ if (h) { enet_host_destroy(h); } }),
  address_{}, is_running_(false), tick_counter_(0)
{
  if (!InitServer(6969, 2)) {
    return;
  }

  is_running_ = true;
}


Server::Server(ENetHost* server, ENetAddress address, bool is_running, int tick_counter)
  : server_(server, [](ENetHost* h){ if(h) { enet_host_destroy(h); } }),
  address_(address), is_running_(is_running), tick_counter_(tick_counter)
{}

Server::~Server() { Stop(); }

int Server::Start()
{
  if (!is_running_ || !server_) { return -1; }

  ENetEvent event;
  auto last_tick = std::chrono::steady_clock::now();

  while (is_running_) {
    while (is_running_ && enet_host_service(server_.get(), &event, 0) > 0) {
      if (event.type == ENET_EVENT_TYPE_CONNECT) {
        ConnectClient(event);
      }
      if (event.type == ENET_EVENT_TYPE_RECEIVE){
        enet_packet_destroy(event.packet);
      } 
      if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
        DisconnectClient(event);
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

    world_simulation_.Update(static_cast<float>(elapsed) / 1000.0F);
  }

  return 0;
}

int Server::Stop()
{
  if (!server_) {
    return -1;
  }

  server_.reset(); 
  enet_deinitialize();
  is_running_ = false;

  return 0;
}

bool Server::ConnectClient(const ENetEvent& event) const
{
  if (!is_running_) { 
    return false;
  }
  
  if (event.peer == nullptr) { 
    return false;
  }

  if(event.type != ENET_EVENT_TYPE_CONNECT) {
    return false;
  }
  
  return true;
}

bool Server::DisconnectClient(const ENetEvent& event)
{
  if (event.type != ENET_EVENT_TYPE_DISCONNECT) {
    return false;
  }
  
  if (!is_running_ || !server_) {
     return false;
  }
  
  if (server_->connectedPeers == 0) {
    is_running_ = false;
  }

  return true;
}

bool Server::InitServer(int port, int max_clients)
{
  if (enet_initialize() != 0) {
    return false;
  }

  address_.host = ENET_HOST_ANY;
  address_.port = static_cast<enet_uint16>(port);

  ENetHost* host = enet_host_create(&address_, static_cast<size_t>(max_clients), 2, 0, 0);
  if (host == nullptr) {
    enet_deinitialize();
    return false;
  }

  server_.reset(host, [](ENetHost* h) {
    if (h) {
      enet_host_destroy(h);
    }
  });

  return true;
}


void Server::BroadcastGameObjectState() const
{
  if (!server_) {
    return;
  }
}

void Server::Run() { Start(); }

} // namespace chroma::server
