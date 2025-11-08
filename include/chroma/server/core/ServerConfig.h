#pragma once

namespace chroma::server::core {

struct ServerConfig
{
  int port = 6969;
  int max_clients = 2;
  int ticks = 20;
};

}// namespace chroma::server::core
