#pragma once

#include <enet.h>
#include <memory>

#include "chroma/app/states/State.h"
#include "chroma/app/states/mediator/GameNetworkMediator.h"
#include "chroma/shared/events/EventDispatcher.h"
#include "chroma/shared/events/Subscription.h"

namespace chroma::app::states {
class NetworkState final : public State
{
public:
  NetworkState();
  explicit NetworkState(std::shared_ptr<GameNetworkMediator> game_mediator);
  ~NetworkState() override;

  NetworkState(const NetworkState &) = delete;
  NetworkState &operator=(const NetworkState &) = delete;
  NetworkState(NetworkState &&) = delete;
  NetworkState &operator=(NetworkState &&) = delete;

  void OnUpdate(float delta_time) override;
  void OnEvent(shared::event::Event &event) override;
  void SetEventDispatcher();

  void SetDispatchEvent(bool dispatch);
  [[nodiscard]] bool GetDispatch() const;

private:
  std::unique_ptr<ENetHost, decltype(&enet_host_destroy)> client_;
  std::unique_ptr<ENetPeer, decltype(&enet_peer_reset)> server_peer_;
  ENetAddress server_address_;
  ENetEvent event_;

  shared::event::Subscription key_sub_;

  bool connected_ { false };
  float delta_time_ { 0.0F };
  bool dispatch_event_ { true };

  std::shared_ptr<GameNetworkMediator> game_mediator_;

  static void PeerDeleter(ENetPeer *peer);
  bool InitNetworkClient();
  bool ConnectToServer(const std::string &host, enet_uint16 port);
  void DisconnectFromServer();
  bool TryConnect(const std::string &host, enet_uint16 port);
  void OnReceiveData() const;
  void ProcessEvent();
};
}// namespace chroma::app::states