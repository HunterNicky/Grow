#pragma once

#include "chroma/app/commands/Command.h"

#include <memory>
#include <queue>

namespace chroma::app::command {
class CommandQueue
{
public:
  void Push(std::unique_ptr<Command> command);
  void Process();
  bool IsEmpty() { return queue_.empty(); }

private:
  std::queue<std::unique_ptr<Command>> queue_;
};
}// namespace chroma::app::command