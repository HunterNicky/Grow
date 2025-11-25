#pragma once

namespace chroma::app::command {
class Command
{
public:
  virtual ~Command() = default;
  virtual void Execute() = 0;
};
}// namespace chroma::app::command