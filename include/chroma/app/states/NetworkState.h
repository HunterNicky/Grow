#pragma once

#include <enet.h>
#include <memory>
// #include <vector>

// #include "chroma/shared/core/GameObject.h"
#include "chroma/app/states/State.h"

namespace chroma::app::layer::states {
class NetworkState : public State
{
public:
    NetworkState();
    ~NetworkState() override;

    NetworkState(const NetworkState&) = delete;
    NetworkState& operator=(const NetworkState&) = delete;
    NetworkState(NetworkState&&) = delete;
    NetworkState& operator=(NetworkState&&) = delete;
    
    void OnUpdate(float delta_time) override;

private:

    std::unique_ptr<ENetHost> client_;
    std::unique_ptr<ENetPeer> server_peer_;
    ENetAddress server_address_;
    ENetEvent event_;

    bool connected_ = false;

    void ConnectToServer(const std::string& host, enet_uint16 port);
    void DisconnectFromServer();
    void OnReceiveData() const;
    void ProcessEvent(const ENetEvent& event);
    void InterpolateGameObjectStates(float delta_time);

    // std::vector<std::shared_ptr<chroma::shared::core::GameObject>> game_objects_past_;
    // std::vector<std::shared_ptr<chroma::shared::core::GameObject>> game_objects_present_;
};
}