#pragma once

#include <cstdint>
#include <enet.h>
#include <memory>
#include <unordered_map>

#include "chroma/server/WorldSimulation.h"

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
  Server(ENetHost *server, const ENetAddress &address, bool is_running, int tick_counter);
  ~Server();
  void ProcessGameEvent(const ENetEvent &event, long long elapsed, const Game::Event *evt, Game::EventUnion event_union);

  int Start();
  int Stop();

  void ConnectClient(const ENetEvent &event);
  void DisconnectClient(const ENetEvent &event);

  bool InitServer(int port, int max_clients);

  [[nodiscard]] bool IsRunning() const { return is_running_; }
  void SetRunning(bool running) { is_running_ = running; }

  void BroadcastGameObjectState(uint64_t delta_time) const;

  void Run();

private:
  std::shared_ptr<ENetHost> server_;
  ENetAddress address_;

  bool is_running_;
  int tick_counter_;

  WorldSimulation world_simulation_;
  std::unordered_map<ENetPeer *, UUIDv4::UUID> connected_players_;
  std::unordered_map<ENetPeer *, uint32_t> peer_last_processed_input_;
};
}// namespace chroma::server
