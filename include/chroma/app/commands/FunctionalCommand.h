#pragma once

#include "chroma/app/commands/Command.h"

#include <functional>

namespace chroma::app::command {
class FunctionalCommand : public Command
{
public:
  explicit FunctionalCommand(std::function<void()> function);
  void Execute() override;

private:
  std::function<void()> function_;
};
}// namespace chroma::app::command