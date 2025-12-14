#pragma once

namespace chroma::server::core {

struct ServerConfig
{
  int port = 6969;
  int max_clients = 2;
  int ticks = 20;

  static ServerConfig Singleplayer()
  {
    return ServerConfig{ .port = 6969, .max_clients = 1, .ticks = 20 };
  }

  static ServerConfig Multiplayer()
  {
    return ServerConfig{ .port = 6969, .max_clients = 2, .ticks = 20 };
  }
};

}// namespace chroma::server::core
