#include "chroma/shared/core/player/Player.h"

#include "chroma/shared/audio/AudioBridge.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/AudioListener.h"
#include "chroma/shared/core/components/Camera.h"
#include "chroma/shared/core/components/Health.h"
#include "chroma/shared/core/components/Movement.h"
#include "chroma/shared/core/components/Speed.h"
#include "chroma/shared/core/components/SpriteAnimation.h"
#include "chroma/shared/core/components/Transform.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/InputState.h"
#include "chroma/shared/events/KeyEvent.h"
#include "chroma/shared/events/SoundEvent.h"
#include "chroma/shared/render/RenderBridge.h"

#include <cmath>
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <string>

namespace chroma::shared::core::player {
Player::Player() { Type_ = GameObjectType::PLAYER; }

void Player::InitComponents()
{
  const auto speed_component = std::make_shared<component::Speed>(50.0F);
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

  const auto listener_component = std::make_shared<component::AudioListener>();
  AttachComponent(listener_component);

  const auto health_component = std::make_shared<component::Health>(100.0F);
  health_component->SetCurrentHealth(1.F);
  AttachComponent(health_component);
  
  SetupAnimation(anim_component);
}

void Player::SetupAnimation(const std::shared_ptr<component::SpriteAnimation> &anim_component)
{
  const std::string filepath = "assets/sprites/player/randi-1.png";

  component::SpriteAnimationDesc idle_down;
  idle_down.name = "idle_down";
  idle_down.loop = false;
  idle_down.frames = {
    { .sprite_id = filepath, .duration_ticks = 60, .subregion = { .x = 1, .y = 1, .width = 16, .height = 32 } }
  };
  anim_component->LoadAnimation("idle_down", idle_down);

  component::SpriteAnimationDesc idle_up;
  idle_up.name = "idle_up";
  idle_up.loop = false;
  idle_up.frames = {
    { .sprite_id = filepath, .duration_ticks = 60, .subregion = { .x = 18, .y = 1, .width = 16, .height = 32 } }
  };
  anim_component->LoadAnimation("idle_up", idle_up);
  
  component::SpriteAnimationDesc punch_side;
  punch_side.name = "punch_side";
  punch_side.loop = false;
  punch_side.frames = {
    { .sprite_id = filepath, .duration_ticks = 60, .subregion = { .x = 526, .y = 132, .width = 22, .height = 29 } },
    { .sprite_id = filepath, .duration_ticks = 60, .subregion = { .x = 548, .y = 132, .width = 24, .height = 29 } },
    { .sprite_id = filepath, .duration_ticks = 60, .subregion = { .x = 572, .y = 132, .width = 25, .height = 29 } }
  };
  anim_component->LoadAnimation("punch_side", punch_side);
  
  component::SpriteAnimationDesc idle_side;
  idle_side.name = "idle_side";
  idle_side.loop = false;
  idle_side.frames = {
    { .sprite_id = filepath, .duration_ticks = 60, .subregion = { .x = 35, .y = 1, .width = 15, .height = 32 } }
  };
  anim_component->LoadAnimation("idle_side", idle_side);
  
  component::SpriteAnimationDesc walk_down;
  walk_down.name = "walk_down";
  walk_down.loop = true;
  walk_down.frames = {
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 51, .y = 1, .width = 18, .height = 30 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 70, .y = 1, .width = 19, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 90, .y = 1, .width = 19, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 110, .y = 1, .width = 18, .height = 31 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 129, .y = 1, .width = 19, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 149, .y = 1, .width = 19, .height = 32 } }
  };
  anim_component->LoadAnimation("walk_down", walk_down);

  component::SpriteAnimationDesc walk_up;
  walk_up.name = "walk_up";
  walk_up.loop = true;
  walk_up.frames = {
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 169, .y = 1, .width = 16, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 186, .y = 1, .width = 18, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 205, .y = 1, .width = 18, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 224, .y = 1, .width = 16, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 241, .y = 1, .width = 18, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 260, .y = 1, .width = 18, .height = 32 } }
  };
  anim_component->LoadAnimation("walk_up", walk_up);

  component::SpriteAnimationDesc walk_side;
  walk_side.name = "walk_side";
  walk_side.loop = true;
  walk_side.frames = {
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 279, .y = 1, .width = 20, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 300, .y = 1, .width = 18, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 319, .y = 1, .width = 17, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 337, .y = 1, .width = 23, .height = 29 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 361, .y = 1, .width = 16, .height = 32 } },
    { .sprite_id = filepath, .duration_ticks = 180, .subregion = { .x = 378, .y = 1, .width = 18, .height = 32 } }
  };
  anim_component->LoadAnimation("walk_side", walk_side);

  anim_component->Play("idle_down", false);
}

Player::~Player() = default;

void Player::AnimationState(const Vector2 dir, const float magnitude)
{
  if (const auto anim = GetComponent<component::SpriteAnimation>()) {
    if (magnitude <= 0.0F) {
      if (was_moving_) {
        step_timer_ = 0.0F;
        was_moving_ = false;
      }
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
}
void Player::OnUpdate(float delta_time)
{
  const auto transform = GetComponent<component::Transform>();
  const auto speed = GetComponent<component::Speed>();
  if (!transform || !speed) { return; }

  const auto movement = GetComponent<component::Movement>();
  if (!movement) { return; }
  
  const auto health = GetComponent<core::component::Health>();
  if (!health) { return; }

  const bool is_authority = HasAuthority();
  const bool is_autonomous = IsAutonomousProxy();
  const bool should_simulate = (is_authority || is_autonomous);
  
  Vector2 dir = movement->GetDirection();
  const float magnitude = Vector2Length(dir);
  
  if (magnitude > 0.0F) {
    dir = Vector2Normalize(dir);
    movement->SetDirection(dir);

    if (should_simulate) {
      Vector2 pos = transform->GetPosition();
      pos.x += dir.x * speed->GetSpeed().x * delta_time;
      pos.y += dir.y * speed->GetSpeed().y * delta_time;
      transform->SetPosition(pos);
    }
    
    if (is_authority) {
      step_timer_ += delta_time;
      constexpr float step_interval = 0.5F;
      if (step_timer_ >= step_interval) {
        step_timer_ = 0.0F;
        auto sound_event =
          event::SoundEvent("step", 0.6F, 0.95F + (static_cast<float>(GetRandomValue(0, 10)) / 100.0F));
        sound_event.SetEmitterId(GetId().str());
        event::EventBus::Dispatch(sound_event);
        
      }
    }
    was_moving_ = true;
  }
  
  health->Heal(10.F * delta_time);

  AnimationState(dir, magnitude);

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

  const auto health = GetComponent<core::component::Health>();
  if (!health) { return; }

  const Vector2 pos = transform->GetPosition();
  const Vector2 scale = transform->GetScale();
  const float rotation = transform->GetRotation();
  const bool flip_x = (last_facing_ == FacingDir::Side) && last_left_;

  bridge->DrawAnimation(*anim, pos, scale, rotation, WHITE, flip_x, false, { 0.5F, 0.5F });

  if(health)
  {
      Vector2 pos_h;
      pos_h.y = pos.y - 21.F;
      pos_h.x = pos.x - 15.F;
      Vector2 size = {.x =30.F, .y = 4.F};
      health->DrawHealth(pos_h, size);
  }
}

void Player::HandleEvent(const event::Event &event)
{
  switch (event.GetType()) {
  case event::Event::KeyEvent: {
    
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
    if(input_state_.IsKeyPressed(KEY_L))  { attacking_ = true; }
    if(!input_state_.IsKeyPressed(KEY_L)) { attacking_ = false; }
    movement->SetDirection(direction);
   
    break;
  }
  case event::Event::SoundEvent: {
    const auto &sound_event = dynamic_cast<const event::SoundEvent &>(event);
    if (const auto ab = audio::GetAudioBridge()) {
      ab->PlaySoundAt(sound_event.GetSoundName(),
        GetComponent<component::Transform>()->GetPosition().x,
        GetComponent<component::Transform>()->GetPosition().y,
        sound_event.GetVolume(),
        sound_event.GetPitch());
    }
    break;
  }
  default:
    break;
  }
}

void Player::UpdateAnimationFromDirection(const Vector2 dir)
{
  const auto anim = GetComponent<component::SpriteAnimation>();
  if (!anim) { return; }

  const float magnitude = Vector2Length(dir);
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

std::shared_ptr<GameObject> Player::Clone() { return std::make_shared<Player>(*this); }

}// namespace chroma::shared::core::player
