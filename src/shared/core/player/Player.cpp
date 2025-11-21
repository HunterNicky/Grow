#include "chroma/shared/core/player/Player.h"

#include "chroma/shared/audio/AudioBridge.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Health.h"
#include "chroma/shared/core/components/Movement.h"
#include "chroma/shared/core/components/Speed.h"
#include "chroma/shared/core/components/SpriteAnimation.h"
#include "chroma/shared/core/components/Transform.h"
#include "chroma/shared/core/components/Inventory.h"
#include "chroma/shared/core/components/Weapon.h"

#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/InputState.h"
#include "chroma/shared/events/KeyEvent.h"
#include "chroma/shared/events/SoundEvent.h"
#include "chroma/shared/render/RenderBridge.h"
#include "chroma/shared/render/SpriteLoader.h"
#include "chroma/shared/core/components/Run.h"

#include <cmath>
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <string>


namespace chroma::shared::core::player {
Player::Player() { Type_ = GameObjectType::PLAYER; }

void Player::SetupAnimation(const std::shared_ptr<component::SpriteAnimation> &anim_component)
{
    render::SpriteLoader::LoadSpriteAnimationFromFile(anim_component, "assets/sprites/player/weapons/randi-fist.json");
    render::SpriteLoader::LoadSpriteAnimationFromFile(anim_component, "assets/sprites/player/weapons/randi-spear.json");
    render::SpriteLoader::LoadSpriteAnimationFromFile(anim_component, "assets/sprites/player/weapons/randi-whip.json");

    anim_component->Play("fist_idle_down", false);
}

Player::~Player() = default;

void Player::AnimationState(const Vector2 dir, const float magnitude)
{
  std::string mode = "fist_";

  auto inventory = GetComponent<component::Inventory>();
  if(inventory)
  {
    auto current_weapon = inventory->GetCurrentWeapon();
    if(current_weapon)
    {
        mode = component::Weapon::WeaponTypeToPrefix(current_weapon->GetWeaponType()) + "_";
    }
  }

  if (const auto anim = GetComponent<component::SpriteAnimation>()) {
    if (magnitude <= 0.0F) {
      if (was_moving_) {
        step_timer_ = 0.0F;
        was_moving_ = false;
      }
      switch (last_facing_) {
      case FacingDir::Up:
        anim->Play( mode + "idle_up", false);
        break;
      case FacingDir::Down:
        anim->Play( mode + "idle_down", false);
        break;
        case FacingDir::Side:
        anim->Play( mode + "idle_side", false);
        break;
      }
    } else {

      bool run = false;

      const auto run_comp = GetComponent<component::Run>();
      if(run_comp)
      {
          run = run_comp->IsRunning();
      }
      
      std::string state = run ? "running_" : "walk_";
      if (std::fabs(dir.x) > std::fabs(dir.y)) {
        last_facing_ = FacingDir::Side;
        last_left_ = (dir.x < 0.0F);
        anim->Play(mode + state + "side", false);
      } else if (dir.y < 0.0F) {
        last_facing_ = FacingDir::Up;
        anim->Play(mode + state + "up", false);
      } else {
        last_facing_ = FacingDir::Down;
        anim->Play(mode + state + "down", false);
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

  const auto run_comp = GetComponent<component::Run>();
  if(!run_comp) { return; }

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

      pos.x += dir.x * speed->GetSpeed().x * run_comp->GetSpeedFactor() * delta_time;
      pos.y += dir.y * speed->GetSpeed().y * run_comp->GetSpeedFactor() * delta_time;

      transform->SetPosition(pos);

      auto inventory = GetComponent<component::Inventory>();
      if(inventory)
      {
          auto weapon = inventory->GetCurrentWeapon();
          if(weapon)
          {
              weapon->SetPosition(pos);
          }
      }
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
  if(is_authority)
  {
    health->Heal(10.F * delta_time);
  }

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

  auto inventory = GetComponent<component::Inventory>();
  
  if(inventory)
  {
      auto weapon = inventory->GetCurrentWeapon();
      if(weapon)
      {
          weapon->SetPosition(pos);
          weapon->Render();
      }
  }

  bridge->DrawAnimation(*anim, pos, scale, rotation, WHITE, flip_x, false, { 0.5F, 0.5F });

  if(health)
  {
      Vector2 pos_h;
      pos_h.y = pos.y - 30.F;
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
   
    input_state_.SetKeyState(key_event.GetKey(), key_event.IsPressed());
   
    Vector2 direction{ 0.0F, 0.0F };
   
    GetComponent<component::Run>()->SetRunning(input_state_.IsKeyPressed(KEY_K));

    if (input_state_.IsKeyPressed(KEY_W)) { direction.y -= 1.0F; }
    if (input_state_.IsKeyPressed(KEY_S)) { direction.y += 1.0F; }
    if (input_state_.IsKeyPressed(KEY_A)) { direction.x -= 1.0F; }
    if (input_state_.IsKeyPressed(KEY_D)) { direction.x += 1.0F; }
    

    if(input_state_.IsKeyPressed(KEY_J))
    {
        auto inventory = GetComponent<component::Inventory>();
        if(inventory)
        {
            const auto weapon = inventory->ChangeToPreviousWeapon();
            SetCurrentWeapon(weapon);
        }
    }
    else if(input_state_.IsKeyPressed(KEY_L))
    {
        auto inventory = GetComponent<component::Inventory>();
        if(inventory)
        {
            const auto weapon = inventory->ChangeToNextWeapon();
            SetCurrentWeapon(weapon);
        }
    }

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

void Player::SetCurrentWeapon(const std::shared_ptr<component::Weapon>& weapon)
{
    if(!weapon) { return; }

    const auto anim = GetComponent<component::SpriteAnimation>();
    if (!anim) { return; }

    const auto inventory = GetComponent<component::Inventory>();
    if(!inventory) { return; }

    inventory->SetCurrentWeapon(weapon);
}

std::shared_ptr<GameObject> Player::Clone() { return std::make_shared<Player>(*this); }
    

}// namespace chroma::shared::core::player
