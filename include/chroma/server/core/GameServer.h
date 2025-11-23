#pragma once

#include <chrono>
#include <enet.h>

#include "chroma/server/core/ServerConfig.h"
#include "chroma/server/core/ServerCore.h"
#include "chroma/server/core/SessionManager.h"
#include "chroma/server/logic/ServerEventSystem.h"
#include "chroma/server/logic/ServerGameLogic.h"
#include "chroma/server/network/ServerNetwork.h"

namespace chroma::server::core {

class GameServer final : public ServerCore
{
public:
  GameServer();
  ~GameServer() override = default;

  int Start() override;
  int Stop() override;

  void ConnectClient(const ENetEvent &event);
  void DisconnectClient(const ENetEvent &event);

  bool InitServer(int port, int max_clients);

  [[nodiscard]] bool IsRunning() const { return is_running_; }
  void SetRunning(const bool running) { is_running_ = running; }

  void BroadcastGameObjectState(uint64_t server_time_ms) const;

private:
  ServerConfig config_{};
  network::ServerNetwork network_;
  logic::ServerGameLogic game_logic_;
  SessionManager sessions_{};
  logic::ServerEventSystem event_system_{};

  bool is_running_ = false;
  int tick_counter_ = 0;

  std::chrono::steady_clock::time_point start_time_{};
  std::chrono::steady_clock::time_point last_frame_time_{};
  std::chrono::steady_clock::time_point last_snapshot_time_{};
};

}// namespace chroma::server::core
