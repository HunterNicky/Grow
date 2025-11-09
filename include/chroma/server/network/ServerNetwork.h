#pragma once

#include <memory>

#include <enet.h>

namespace chroma::server::network {

class ServerNetwork
{
public:
  ServerNetwork();
  ~ServerNetwork();

  bool InitServer(int port, int max_clients);
  void Stop();

  bool PollEvent(ENetEvent &event, int timeout_ms) const;
  void Send(ENetPeer *peer, const uint8_t *data, size_t size, bool reliable = true) const;
  void Flush() const;

  [[nodiscard]] ENetHost *GetHost() const { return host_.get(); }
  [[nodiscard]] const ENetAddress &GetAddress() const { return address_; }
  [[nodiscard]] bool IsReady() const { return host_ != nullptr; }
  [[nodiscard]] size_t ConnectedPeers() const { return host_ ? host_->connectedPeers : 0; }

private:
  std::shared_ptr<ENetHost> host_;
  ENetAddress address_{};
};

}// namespace chroma::server::network
