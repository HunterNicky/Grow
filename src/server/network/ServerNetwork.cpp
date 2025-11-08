#include "chroma/server/network/ServerNetwork.h"

namespace chroma::server::network {

ServerNetwork::ServerNetwork() = default;

ServerNetwork::~ServerNetwork() { Stop(); }

bool ServerNetwork::InitServer(const int port, const int max_clients)
{
  if (enet_initialize() != 0) { return false; }

  address_.host = ENET_HOST_ANY;
  address_.port = static_cast<enet_uint16>(port);

  ENetHost *host = enet_host_create(&address_, static_cast<size_t>(max_clients), 2, 0, 0);
  if (host == nullptr) {
    enet_deinitialize();
    return false;
  }

  host_ = std::shared_ptr<ENetHost>(host, [](ENetHost *h) {
    if (h) { enet_host_destroy(h); }
  });

  return true;
}

void ServerNetwork::Stop()
{
  if (host_) { host_.reset(); }
  enet_deinitialize();
}

bool ServerNetwork::PollEvent(ENetEvent &event, const int timeout_ms) const
{
  if (!host_) { return false; }
  return enet_host_service(host_.get(), &event, timeout_ms) > 0;
}

void ServerNetwork::Send(ENetPeer *peer, const uint8_t *data, const size_t size, const bool reliable) const
{
  if (!peer || !host_) { return; }
  const enet_uint32 flags = reliable ? ENET_PACKET_FLAG_RELIABLE : 0;
  ENetPacket *packet = enet_packet_create(data, size, flags);
  enet_peer_send(peer, 0, packet);
}

void ServerNetwork::Flush() const
{
  if (host_) { enet_host_flush(host_.get()); }
}

}// namespace chroma::server::network

