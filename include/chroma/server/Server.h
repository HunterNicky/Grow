#pragma once

#include <enet/enet.h>
#include <memory>
#include <vector>

#include "chroma/shared/core/GameObject.h"

#define TICKS 20

namespace chroma::server {
class Server
{
public:
  Server();
  Server(const Server &) = default;
  Server(Server &&) = delete;
  Server &operator=(const Server &) = default;
  Server &operator=(Server &&) = delete;
  Server(ENetHost *server,
    ENetAddress address,
    bool is_running,
    int tick_counter,
    std::vector<std::shared_ptr<chroma::shared::core::GameObject>> game_objects);
  ~Server();

  int Start();
  int Stop();

  [[nodiscard]] bool ConnectClient(const ENetEvent &event) const;
  bool DisconnectClient(const ENetEvent &event);

  bool InitServer(int port, int max_clients);
  bool CreateGameWorld(/*Enum gameType::LevelMeadow*/);

  [[nodiscard]] bool IsRunning() const { return is_running_; }
  void SetRunning(bool running) { is_running_ = running; }

  void OnUpdate(float delta_time);

  // void HandleLag();
  void BroadcastGameObjectState() const;

  void run();

private:
  ENetHost *server_;// NÃO USE PONTEIRO CRU
  ENetAddress address_;

  bool is_running_;
  int tick_counter_;

  std::vector<std::shared_ptr<chroma::shared::core::GameObject>> game_objects_;
};
}// namespace chroma::server
