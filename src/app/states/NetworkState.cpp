#include "chroma/app/states/NetworkState.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/KeyEvent.h"
#include "chroma/shared/events/MouseEvent.h"
#include "GameObject_generated.h"
#include "chroma/app/states/State.h"
#include "chroma/server/Server.h"
#include "flatbuffers/flatbuffer_builder.h"

#include <thread>
#include <memory>
#include <enet.h>
#include <future>

namespace chroma::app::layer::states {

void NetworkState::PeerDeleter(ENetPeer* peer) {
    if (peer != nullptr && peer->host != nullptr) {
        enet_peer_reset(peer);
    }
}


NetworkState::NetworkState()
    : State("NetworkState"),
      client_(nullptr, &enet_host_destroy),
      server_peer_(nullptr, &PeerDeleter),
      server_address_({}),
      event_({})
{
    if (enet_initialize() != 0) {
        return;
    }

    client_ = std::unique_ptr<ENetHost, decltype(&enet_host_destroy)>(enet_host_create(nullptr, 1, 2, 0, 0), &enet_host_destroy);
    if (client_ == nullptr) {
        enet_deinitialize();
    } 

    ConnectToServer("127.0.0.1", 6969);
}

NetworkState::~NetworkState() {
    DisconnectFromServer();
    client_.reset();
    enet_deinitialize();
}

void NetworkState::DisconnectFromServer() {
    if (!server_peer_) { return; }

    enet_peer_disconnect(server_peer_.get(), 0);

    bool disconnected = false;
    while (enet_host_service(client_.get(), &event_, 3000) > 0) {
        if (event_.type == ENET_EVENT_TYPE_DISCONNECT && event_.peer == server_peer_.get()) {
            disconnected = true;
            break;
        }
    }

    if (!disconnected && server_peer_) {
        enet_peer_reset(server_peer_.get());
    }

    server_peer_.reset();
    connected_ = false;
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

// NOLINTNEXTLINE
bool NetworkState::TryConnect(const std::string& host, enet_uint16 port)
{
    server_peer_.reset();
    ENetEvent event;

    server_address_.port = port;
    enet_address_set_host(&server_address_, host.c_str());

    client_ = std::unique_ptr<ENetHost, decltype(&enet_host_destroy)>(
        enet_host_create(nullptr, 1, 2, 0, 0),
        &enet_host_destroy
    );

    if (!client_) { return false; }

    server_peer_ = std::unique_ptr<ENetPeer, decltype(&enet_peer_reset)>(
        enet_host_connect(client_.get(), &server_address_, 2, 0),
        &PeerDeleter
    );
    enet_host_flush(client_.get());

    return (enet_host_service(client_.get(), &event, 1000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT);
}

void NetworkState::ConnectToServer(const std::string& host, enet_uint16 port) {
    if (!TryConnect(host, port)) {
        std::promise<bool> ready;
        auto fut = ready.get_future();

        std::thread thread_server([&ready]() {
            auto server = std::make_shared<chroma::server::Server>();
            ready.set_value(server->IsRunning());
            server->Run();
        });
        thread_server.detach();

        if (fut.wait_for(std::chrono::seconds(2)) == std::future_status::ready && fut.get()) {
            TryConnect(host, port);
        } 
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

void NetworkState::InterpolateGameObjectStates(float delta_time) {
    (void)delta_time;
}

void NetworkState::OnEvent(shared::event::Event& event) {
    if (!IsActive()) {
        return;
    }

    flatbuffers::FlatBufferBuilder builder(1024);

    Game::InputEventType type = Game::InputEventType::NONE;
    Game::InputEvent event_type = Game::InputEvent::NONE;
    flatbuffers::Offset<void> event_union;

    switch (event.GetType()) {
        case shared::event::Event::KeyEvent: {
            const auto& key_event = dynamic_cast<const shared::event::KeyEvent&>(event);
            auto fb_key_event = Game::CreateKeyEvent(
                builder,
                key_event.GetKey(),
                key_event.IsPressed(),
                key_event.IsReleased()
            );
            type = Game::InputEventType::KEYEVENT;
            event_type = Game::InputEvent::KeyEvent;
            event_union = fb_key_event.Union();
            break;
        }

        case shared::event::Event::MouseClickEvent: {
            const auto& mouse_event = dynamic_cast<const shared::event::MouseEvent&>(event);
            auto fb_mouse_pos = Game::CreateVec2(
                builder,
                mouse_event.GetMousePosition().x,
                mouse_event.GetMousePosition().y
            );
            auto fb_mouse_event = Game::CreateMouseEvent(
                builder,
                mouse_event.IsLeftButtonPressed(),
                mouse_event.IsRightButtonPressed(),
                fb_mouse_pos
            );
            type = Game::InputEventType::MOUSEEVENT;
            event_type = Game::InputEvent::MouseEvent;
            event_union = fb_mouse_event.Union();
            break;
        }

        default:
            return;
    }

    auto fb_input_msg = Game::CreateInputMessage(builder,0, 0.0F,builder.CreateString("PlayerTest"),
                                                type, event_type, event_union );

    auto fb_envelope = Game::CreateEnvelope(builder,Game::MsgType::INPUT,
                                                     Game::MsgUnion::InputMessage,fb_input_msg.Union());

    builder.Finish(fb_envelope);

    auto buf = builder.Release();
    ENetPacket* packet = enet_packet_create(
        buf.data(),
        buf.size(),
        ENET_PACKET_FLAG_RELIABLE
    );

    enet_peer_send(server_peer_.get(), 0, packet);
    enet_host_flush(client_.get());
}


} // namespace chroma::app::layer::states
