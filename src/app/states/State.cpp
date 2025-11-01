#include <string>
#include <utility>

#include "chroma/app/states/State.h"
#include "chroma/shared/events/Event.h"

namespace chroma::app::states {
State::State(std::string name) : name_(std::move(name)), active_(false) {}

const std::string &State::GetName() const { return name_; }

void State::SetActive(const bool active) { active_ = active; }

bool State::IsActive() const { return active_; }

void State::OnAttach() {}

void State::OnDetach() {}

void State::OnUpdate([[maybe_unused]] const float delta_time) {}

void State::OnFixedUpdate([[maybe_unused]] const float fixed_delta_time) {}

void State::OnRender() {}

void State::OnEvent([[maybe_unused]] shared::event::Event &event) {}
}// namespace chroma::app::states
