#include "chroma/shared/core/player/Player.h"

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Movement.h"
#include "chroma/shared/core/components/Speed.h"
#include "chroma/shared/core/components/Transform.h"
#include "chroma/shared/core/components/Color.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/InputState.h"
#include "chroma/shared/events/KeyEvent.h"

#include <cmath>
#include <memory>
#include <raylib.h>
#include <random>

namespace chroma::shared::core::player {
Player::Player() { Type_ = GameObjectType::PLAYER; }

void Player::InitComponents()
{
  auto speed_component = std::make_shared<component::Speed>(20.0F);
  auto movement_component = std::make_shared<component::Movement>();
  AttachComponent(speed_component);
  AttachComponent(movement_component);
  transform_->SetScale({ 50.0F, 50.0F });
  auto color_component = std::make_shared<component::Color>();
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dist(0.0F, 1.0F);

  color_component->SetColor(
      dist(gen),
      dist(gen),
      dist(gen),
      1.0F
  );

  AttachComponent(color_component);
  AttachComponent(transform_);

}

Player::~Player() = default;

void Player::OnUpdate(float delta_time)
{
  auto transform = GetComponent<component::Transform>();
  auto speed = GetComponent<component::Speed>();
  if (!transform || !speed) { return; }

  auto movement = GetComponent<component::Movement>();
  if (!movement || (movement->GetDirection().x == 0.0F && movement->GetDirection().y == 0.0F)) { return; }

  const float magnitude = std::sqrt((movement->GetDirection().x * movement->GetDirection().x)
                                    + (movement->GetDirection().y * movement->GetDirection().y));

  if (magnitude > 0.0F) {
    movement->SetDirection({ movement->GetDirection().x / magnitude, movement->GetDirection().y / magnitude });
  }

  Vector2 pos = transform->GetPosition();
  pos.x += movement->GetDirection().x * speed->GetSpeed().x * delta_time;
  pos.y += movement->GetDirection().y * speed->GetSpeed().y * delta_time;
  transform->SetPosition(pos);
}

void Player::OnFixedUpdate(float fixed_delta_time) { (void)fixed_delta_time; }

void Player::OnCollision(const collision::CollisionEvent &event) { (void)event; }

void Player::OnRender()
{
  auto transform = GetComponent<component::Transform>();
  if (!transform) { return; }

  auto color_component = GetComponent<component::Color>();

  if (!color_component) { return; }

  Color color = {
      static_cast<uint8_t>(color_component->GetRed() * 255.0F),
      static_cast<uint8_t>(color_component->GetGreen() * 255.0F),
      static_cast<uint8_t>(color_component->GetBlue() * 255.0F),
      static_cast<uint8_t>(color_component->GetAlpha() * 255.0F)
  };
  DrawRectangleV(transform->GetPosition(), transform->GetScale(), color);
  
  DrawRectangleLinesEx(
      { transform->GetPosition().x, transform->GetPosition().y, transform->GetScale().x, transform->GetScale().y },
      2.0F,
      BLACK);
}

void Player::HandleEvent(const shared::event::Event &event)
{
    if (event.GetType() != shared::event::Event::KeyEvent) { return; }

    const auto &key_event = dynamic_cast<const shared::event::KeyEvent &>(event);
    auto movement = GetComponent<component::Movement>();
    if (!movement) { return; }
    
    if (key_event.IsPressed()) {
        input_state_.SetKeyState(key_event.GetKey(), true);
    } else if (key_event.IsReleased()) {
        input_state_.SetKeyState(key_event.GetKey(), false);
    }
    
    Vector2 direction{ 0.0F, 0.0F };
    if (input_state_.IsKeyPressed(KEY_W)) { direction.y -= 1.0F; }
    if (input_state_.IsKeyPressed(KEY_S)) { direction.y += 1.0F; }
    if (input_state_.IsKeyPressed(KEY_A)) { direction.x -= 1.0F; }
    if (input_state_.IsKeyPressed(KEY_D)) { direction.x += 1.0F; }

    movement->SetDirection(direction);
}

std::shared_ptr<GameObject> Player::Clone()
{
  return std::make_shared<Player>(*this);
}

}// namespace chroma::shared::core::player
