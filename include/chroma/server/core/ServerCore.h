#pragma once

namespace chroma::server::core {

class ServerCore
{
public:
  virtual ~ServerCore() = default;

  virtual int Start() = 0;
  virtual int Stop() = 0;

  virtual void Run() { Start(); }
};

}// namespace chroma::server::core
