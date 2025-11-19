#include "chroma/app/commands/CommandQueue.h"
#include "chroma/app/commands/Command.h"

#include <memory>
#include <utility>
#include <queue>

namespace chroma::app::command {
  void CommandQueue::Push(std::unique_ptr<Command> command){
    queue_.push(std::move(command));
  }

  void CommandQueue::Process()
  {
    while (!queue_.empty())
    {
      const std::unique_ptr<Command> cmd = std::move(queue_.front());
      queue_.pop();
      cmd->Execute();
    }
  }
}