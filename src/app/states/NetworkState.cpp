#include "chroma/app/states/NetworkState.h"
#include "chroma/app/states/State.h"

#include <memory>
#include <enet.h>

namespace chroma::app::layer::states {

NetworkState::NetworkState()
    : State("NetworkState"),
      client_(nullptr),
      server_peer_(nullptr),
      server_address_({}),
      event_({})
{
    if (enet_initialize() != 0) {
        return;
    }

    client_ = std::unique_ptr<ENetHost>(enet_host_create(nullptr, 1, 2, 0, 0));
    if (client_ == nullptr) {
        enet_deinitialize();
    } 
}

NetworkState::~NetworkState() {
    DisconnectFromServer();
    client_.reset();
    enet_deinitialize();
}

void NetworkState::OnUpdate(float delta_time) {

    while (enet_host_service(client_.get(), &event_, 0) > 0) {
        ProcessEvent(event_);
    }

    InterpolateGameObjectStates(delta_time);
}

void NetworkState::OnReceiveData() const {
    if (event_.type == ENET_EVENT_TYPE_RECEIVE) {
        //Adicionar conversão de pacote e numero de recebimento
        enet_packet_destroy(event_.packet);
    }
}

void NetworkState::ConnectToServer(const std::string& host, enet_uint16 port) {
    if (client_ == nullptr) {
        return;
    }

    enet_address_set_host(&server_address_, host.c_str());
    server_address_.port = port;

    server_peer_ = std::unique_ptr<ENetPeer>(enet_host_connect(client_.get(), &server_address_, 2, 0));
    if (server_peer_ == nullptr) {
        return;
    }

    if (enet_host_service(client_.get(), &event_, 5000) > 0 &&
        event_.type == ENET_EVENT_TYPE_CONNECT) {
    } else {
        enet_peer_reset(server_peer_.get());
        server_peer_ = nullptr;
    }
}

void NetworkState::DisconnectFromServer() {
    if (server_peer_ != nullptr) {
        enet_peer_disconnect(server_peer_.get(), 0);
        while (enet_host_service(client_.get(), &event_, 3000) > 0) {
            if (event_.type == ENET_EVENT_TYPE_RECEIVE) {
                enet_packet_destroy(event_.packet);
            } else if (event_.type == ENET_EVENT_TYPE_DISCONNECT) {
                break;
            }
        }
        server_peer_ = nullptr;
    }
}

void NetworkState::ProcessEvent(const ENetEvent& event) {
    switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT:
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            OnReceiveData();
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
            server_peer_ = nullptr;
            break;
        default:
            break;
    }
}

// NOLINTNEXTLINE
void NetworkState::InterpolateGameObjectStates(float delta_time) {
    (void)delta_time;
}

} // namespace chroma::app::layer::states
