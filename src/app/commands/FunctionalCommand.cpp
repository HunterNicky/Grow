#include "chroma/app/commands/FunctionalCommand.h"

#include <functional>
#include <utility>

namespace chroma::app::command {
FunctionalCommand::FunctionalCommand(std::function<void()> function) : function_(std::move(function)) {}

void FunctionalCommand::Execute() { function_(); }
}// namespace chroma::app::command