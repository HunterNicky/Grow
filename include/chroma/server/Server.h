#pragma once

#include <vector>
#include "enet.h"
#include <memory>

#include "chroma/shared/core/GameObject.h"

#define TICKS 20

namespace chroma::server {
class Server
{
public:
  Server();
  ~Server();

  int start();
  int stop();

  bool connectClient(ENetEvent& event) const;
  bool disconnectClient(ENetEvent& event);

  bool initServer(int port, int maxClients);
  bool CreateGameWorld(/*Enum gameType::LevelMeadow*/);

  bool isRunning() const { return isRunning_; }
  void SetRunning(bool running) { isRunning_ = running; }

  void OnUpdate(float delta_time);
  
  //void HandleLag();
  void BroadcastGameObjectState();

  void run();

private:
  ENetHost* server_;
  ENetAddress address_;
  
  bool isRunning_;
  int tickCounter;

  std::vector<std::shared_ptr<chroma::shared::core::GameObject>> game_objects_{};
};
}// namespace chroma::server
