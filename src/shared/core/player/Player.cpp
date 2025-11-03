#include "chroma/shared/core/player/Player.h"

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Camera.h"
#include "chroma/shared/core/components/Coloring.h"
#include "chroma/shared/core/components/Movement.h"
#include "chroma/shared/core/components/Speed.h"
#include "chroma/shared/core/components/SpriteAnimation.h"
#include "chroma/shared/core/components/Transform.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/InputState.h"
#include "chroma/shared/events/KeyEvent.h"
#include "chroma/shared/render/RenderBridge.h"

#include <cmath>
#include <cstdint>
#include <memory>
#include <random>
#include <raylib.h>
#include <raymath.h>
#include <string>

namespace chroma::shared::core::player {
Player::Player() { Type_ = GameObjectType::PLAYER; }

void Player::InitComponents()
{
  const auto speed_component = std::make_shared<component::Speed>(20.0F);
  const auto movement_component = std::make_shared<component::Movement>();
  AttachComponent(speed_component);
  AttachComponent(movement_component);
  transform_->SetScale({ 1.0F, 1.0F });
  AttachComponent(transform_);

  const auto anim_component = std::make_shared<component::SpriteAnimation>();
  AttachComponent(anim_component);

  const auto cam_component = std::make_shared<component::CameraComponent>();
  cam_component->SetMode(render::CameraMode::FollowSmooth);
  cam_component->SetZoom(3.0F);
  cam_component->SetSmoothness(2.0F);
  cam_component->SetDeadzone({ .x = 64.0F, .y = 128.0F });
  AttachComponent(cam_component);

  SetupAnimation(anim_component);
}

void Player::SetupAnimation(const std::shared_ptr<component::SpriteAnimation> &anim_component)
{
  const std::string filepath = "assets/sprites/player/randi-1.png";

  component::SpriteAnimationDesc idle_down;
  idle_down.name = "idle_down";
  idle_down.loop = false;
  idle_down.frames = {
    { .sprite_id = filepath, .duration_ticks = 60, .subregion = { .x = 0, .y = 0, .width = 16, .height = 32 } }
  };
  anim_component->LoadAnimation("idle_down", idle_down);

  component::SpriteAnimationDesc idle_up;
  idle_up.name = "idle_up";
  idle_up.loop = false;
  idle_up.frames = {
    { .sprite_id = filepath, .duration_ticks = 60, .subregion = { .x = 16, .y = 0, .width = 16, .height = 32 } }
  };
  anim_component->LoadAnimation("idle_up", idle_up);

  component::SpriteAnimationDesc idle_side;
  idle_side.name = "idle_side";
  idle_side.loop = false;
  idle_side.frames = {
    { .sprite_id = filepath, .duration_ticks = 60, .subregion = { .x = 34, .y = 0, .width = 15, .height = 32 } }
  };
  anim_component->LoadAnimation("idle_side", idle_side);

  component::SpriteAnimationDesc walk_down;
  walk_down.name = "walk_down";
  walk_down.loop = true;
  walk_down.frames = {
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 64, .y = 0, .width = 18, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 83, .y = 0, .width = 19, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 103, .y = 0, .width = 19, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 123, .y = 0, .width = 18, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 142, .y = 0, .width = 19, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 162, .y = 0, .width = 19, .height = 32 } }
  };
  anim_component->LoadAnimation("walk_down", walk_down);

  component::SpriteAnimationDesc walk_up;
  walk_up.name = "walk_up";
  walk_up.loop = true;
  walk_up.frames = {
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 192, .y = 0, .width = 16, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 209, .y = 0, .width = 18, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 228, .y = 0, .width = 18, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 247, .y = 0, .width = 16, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 264, .y = 0, .width = 18, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 283, .y = 0, .width = 18, .height = 32 } }
  };
  anim_component->LoadAnimation("walk_up", walk_up);

  component::SpriteAnimationDesc walk_side;
  walk_side.name = "walk_side";
  walk_side.loop = true;
  walk_side.frames = {
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 320, .y = 0, .width = 20, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 341, .y = 0, .width = 19, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 360, .y = 0, .width = 19, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 378, .y = 0, .width = 20, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 402, .y = 0, .width = 19, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 419, .y = 0, .width = 19, .height = 32 } }
  };
  anim_component->LoadAnimation("walk_side", walk_side);

  anim_component->Play("idle_down", false);
}

Player::~Player() = default;

void Player::OnUpdate(float delta_time)
{
  const auto transform = GetComponent<component::Transform>();
  const auto speed = GetComponent<component::Speed>();
  const auto cam = GetComponent<component::CameraComponent>();
  if (!transform || !speed || !cam) { return; }

  const auto movement = GetComponent<component::Movement>();
  if (!movement) { return; }

  Vector2 dir = movement->GetDirection();
  const float magnitude = Vector2Length(dir);

  if (magnitude > 0.0F) {
    dir = Vector2Normalize(dir);
    movement->SetDirection(dir);
    Vector2 pos = transform->GetPosition();
    pos.x += dir.x * speed->GetSpeed().x * delta_time;
    pos.y += dir.y * speed->GetSpeed().y * delta_time;
    transform->SetPosition(pos);
  }

  const auto anim = GetComponent<component::SpriteAnimation>();
  if (anim) {
    if (magnitude <= 0.0F) {
      switch (last_facing_) {
      case FacingDir::Up:
        anim->Play("idle_up", false);
        break;
      case FacingDir::Down:
        anim->Play("idle_down", false);
        break;
      case FacingDir::Side:
        anim->Play("idle_side", false);
        break;
      }
    } else {
      if (std::fabs(dir.x) > std::fabs(dir.y)) {
        last_facing_ = FacingDir::Side;
        last_left_ = (dir.x < 0.0F);
        anim->Play("walk_side", false);
      } else if (dir.y < 0.0F) {
        last_facing_ = FacingDir::Up;
        anim->Play("walk_up", false);
      } else {
        last_facing_ = FacingDir::Down;
        anim->Play("walk_down", false);
      }
    }
  }

  for (const auto &[fst, snd] : components_) { snd->Update(delta_time); }
}

void Player::OnFixedUpdate(float fixed_delta_time) { (void)fixed_delta_time; }

void Player::OnCollision(const collision::CollisionEvent &event) { (void)event; }

void Player::OnRender()
{
  const auto transform = GetComponent<component::Transform>();
  if (!transform) { return; }

  const auto anim = GetComponent<component::SpriteAnimation>();
  if (!anim) { return; }

  const auto bridge = render::GetRenderBridge();
  if (!bridge) { return; }

  const Vector2 pos = transform->GetPosition();
  const Vector2 scale = transform->GetScale();
  const float rotation = transform->GetRotation();
  const bool flip_x = (last_facing_ == FacingDir::Side) && last_left_;

  bridge->DrawAnimation(*anim, pos, scale, rotation, WHITE, flip_x, false, { 0.5F, 0.5F });
}

void Player::HandleEvent(const event::Event &event)
{
  if (event.GetType() != event::Event::KeyEvent) { return; }

  const auto &key_event = dynamic_cast<const event::KeyEvent &>(event);
  const auto movement = GetComponent<component::Movement>();
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

}// namespace chroma::shared::core::player
