#include <chroma/server/Server.h>
#include "enet.h"
#include <cstdlib>
#include <chrono>

namespace chroma::server {

Server::Server() : server_(nullptr), isRunning_(false), tickCounter(0), address_()
{
  if (!initServer(8080, 2))
  {
    isRunning_ = false;
    return;
  }
  else
  {
    isRunning_ = true;
  }
}

Server::~Server() {
  stop();
}

int Server::start() {
  if (!isRunning_ || server_ == nullptr) {
    return -1;
  }

  ENetEvent event;

  auto lastTick = std::chrono::steady_clock::now();

  while (isRunning_)
  {
    while (enet_host_service(server_, &event, 0) > 0)
    {
      switch (event.type)
      {
        case ENET_EVENT_TYPE_CONNECT:
          connectClient(event);
          break;

        case ENET_EVENT_TYPE_RECEIVE:
          enet_packet_destroy(event.packet);
          break;
    
        case ENET_EVENT_TYPE_DISCONNECT:
          disconnectClient(event);
          break;

        default:
          break;
      }
    }

    tickCounter++;
    
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTick).count();
    
    if (tickCounter >= TICKS)
    {
      lastTick = now;
      tickCounter = 0;
      BroadcastGameObjectState();
    }

    OnUpdate(static_cast<float>(elapsed) / 1000.0F);
  }
  return 0;
}

int Server::stop() {
  if (server_ == nullptr) {
    enet_host_destroy(server_);
    server_ = nullptr;
  }
  enet_deinitialize();
  isRunning_ = false;
  return 0;
}

bool Server::connectClient(ENetEvent& event) const{ 
  if (!isRunning()) {
    return false;
  }

  if (event.peer == nullptr || event.type != ENET_EVENT_TYPE_CONNECT)
  {
    return false;
  }
  
  event.peer->data = (void*)"Cliente conectado";
  return true;
}

bool Server::disconnectClient(ENetEvent& event) {
  if (event.type != ENET_EVENT_TYPE_DISCONNECT)
  {
    return false;
  }

  event.peer->data = nullptr;

  if ((server_ == nullptr) && (server_->connectedPeers == 0))
  {
    SetRunning(false);
  }

  return true;
}

bool Server::initServer(int port, int maxClients) {
  if (enet_initialize() != 0) {
    return false;
  }

  address_.host = ENET_HOST_ANY;
  address_.port = port;
  server_ = enet_host_create(&address_, maxClients, 2, 0, 0);

  if (server_ == nullptr) {
    enet_deinitialize();
    return false; 
  }

  if (atexit(enet_deinitialize) != 0) {
    enet_host_destroy(server_);
    enet_deinitialize();
    server_ = nullptr;
    return false;
  }

  return true;
}

void Server::OnUpdate(float delta_time) {
  for(auto& gameObject : game_objects_) {
    if(gameObject->IsActive()) {
      gameObject->OnUpdate(delta_time);
    }
  }
}

void Server::BroadcastGameObjectState() {
  
}

void Server::run() {
  start();
}

} // namespace chroma::server
