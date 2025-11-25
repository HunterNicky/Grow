#include "chroma/shared/core/player/Player.h"

#include "chroma/shared/core/components/world/ColliderBox.h"
#include "chroma/shared/audio/AudioBridge.h"
#include "chroma/shared/collision/CollisionManager.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Attack.h"
#include "chroma/shared/core/components/Camera.h"
#include "chroma/shared/core/components/Health.h"
#include "chroma/shared/core/components/Inventory.h"
#include "chroma/shared/core/components/Movement.h"
#include "chroma/shared/core/components/ProjectileType.h"
#include "chroma/shared/core/components/Speed.h"
#include "chroma/shared/core/components/SpriteAnimation.h"
#include "chroma/shared/core/components/Transform.h"
#include "chroma/shared/core/components/Weapon.h"

#include "chroma/shared/core/components/Run.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/InputState.h"
#include "chroma/shared/events/KeyEvent.h"
#include "chroma/shared/events/ProjectileEvent.h"
#include "chroma/shared/events/SoundEvent.h"
#include "chroma/shared/render/RenderBridge.h"
#include "chroma/shared/render/SpriteLoader.h"
#include "chroma/shared/utils/UUID.h"


#include <cmath>
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <string>


namespace chroma::shared::core::player {
Player::Player() { type_ = GameObjectType::PLAYER; }

void Player::SetupAnimation(const std::shared_ptr<component::SpriteAnimation> &anim_component)
{
  render::SpriteLoader::LoadSpriteAnimationFromFile(anim_component, "assets/sprites/player/weapons/randi-fist.json");
  render::SpriteLoader::LoadSpriteAnimationFromFile(anim_component, "assets/sprites/player/weapons/randi-spear.json");
  render::SpriteLoader::LoadSpriteAnimationFromFile(anim_component, "assets/sprites/player/weapons/randi-javelin.json");

  anim_component->Play("fist_idle_down", false);
}

Player::~Player() = default;

void Player::AnimationState(const Vector2 dir, const float magnitude)
{
  std::string mode = "fist_";

  const auto attack_comp = GetComponent<component::Attack>();
  const auto inventory = GetComponent<component::Inventory>();
  if (inventory) {
    const auto current_weapon = inventory->GetCurrentWeapon();
    if (current_weapon) { mode = component::Weapon::WeaponTypeToPrefix(current_weapon->GetWeaponType()) + "_"; }
  }

  if (const auto anim = GetComponent<component::SpriteAnimation>()) {
    if (magnitude <= 0.0F) {
      if (was_moving_) {
        step_timer_ = 0.0F;
        was_moving_ = false;
      }

      if (attack_comp && attack_comp->IsAttacking()) {
        AnimateAttack(mode, last_facing_);
        return;
      }

      switch (last_facing_) {
      case FacingDir::Up:
        anim->Play(mode + "idle_up", false);
        break;
      case FacingDir::Down:
        anim->Play(mode + "idle_down", false);
        break;
      case FacingDir::Side:
        anim->Play(mode + "idle_side", false);
        break;
      }
    } else {

      bool run = false;

      const auto run_comp = GetComponent<component::Run>();
      if (run_comp) { run = run_comp->IsRunning(); }

      const std::string state = run ? "running_" : "walk_";
      AnimateMove(mode, state, dir);
    }
  }
}

void Player::AnimateAttack(const std::string &mode, const FacingDir facing_dir) const
{
  if (const auto anim = GetComponent<component::SpriteAnimation>()) {
    switch (facing_dir) {
    case FacingDir::Up:
      anim->Play(mode + "attack_up", false);
      break;
    case FacingDir::Down:
      anim->Play(mode + "attack_down", false);
      break;
    case FacingDir::Side:
      anim->Play(mode + "attack_side", false);
      break;
    }
  }
}

void Player::AnimateMove(const std::string &mode, const std::string &state, const Vector2 dir)
{
  if (const auto anim = GetComponent<component::SpriteAnimation>()) {
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

void Player::OnUpdate(const float delta_time)
{
  const auto transform = GetComponent<component::Transform>();
  const auto speed = GetComponent<component::Speed>();
  if (!transform || !speed) { return; }

  const auto movement = GetComponent<component::Movement>();
  if (!movement) { return; }

  const auto health = GetComponent<component::Health>();
  if (!health) { return; }

  const auto run_comp = GetComponent<component::Run>();
  if (!run_comp) { return; }

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

      const auto inventory = GetComponent<component::Inventory>();
      if (inventory) {
        const auto weapon = inventory->GetCurrentWeapon();
        if (weapon) { weapon->SetPosition(pos); }
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
  if (is_authority) { health->Heal(10.F * delta_time); }

  UpdateAttack(delta_time);
  AnimationState(dir, magnitude);
  UpdateColliderSize();

  for (const auto &[fst, snd] : components_) { snd->Update(delta_time); }

  const auto camera = GetComponent<component::CameraComponent>();
  if (camera) {
    const float wheel_move = GetMouseWheelMove();
    if (wheel_move != 0.0F) {
      float new_zoom = camera->GetZoom() + (wheel_move * 0.1F);
      new_zoom = std::clamp(new_zoom, 0.5F, 5.0F);
      camera->SetZoom(new_zoom);
    }
  }
}

void Player::OnFixedUpdate(const float fixed_delta_time) { (void)fixed_delta_time; }

void Player::OnCollision(const collision::CollisionEvent &event) { (void)event; }

void Player::OnRender()
{
  const auto transform = GetComponent<component::Transform>();
  if (!transform) { return; }

  const auto anim = GetComponent<component::SpriteAnimation>();
  if (!anim) { return; }

  const auto bridge = render::GetRenderBridge();
  if (!bridge) { return; }

  const auto health = GetComponent<component::Health>();
  if (!health) { return; }

  const Vector2 pos = transform->GetPosition();
  const Vector2 scale = transform->GetScale();
  const float rotation = transform->GetRotation();
  const bool flip_x = (last_facing_ == FacingDir::Side) && last_left_;

  auto inventory = GetComponent<component::Inventory>();

  bridge->DrawAnimation(*anim, pos, scale, rotation, WHITE, flip_x, false, { 0.5F, 0.5F });

  if (health) {
    Vector2 pos_h;
    pos_h.y = pos.y - 30.F;
    pos_h.x = pos.x - 15.F;
    const Vector2 size = { .x = 30.F, .y = 4.F };
    health->DrawHealth(pos_h, size);
  }

  const auto collider = GetComponent<component::ColliderBox>();
  if (collider) { collider->Render(); }
}

void Player::HandleEvent(event::Event &event)
{
  if (event.IsHandled()) { return; }

  switch (event.GetType()) {
  case event::Event::KeyEvent: {

    const auto &key_event = dynamic_cast<const event::KeyEvent &>(event);

    const auto movement = GetComponent<component::Movement>();
    if (!movement) { return; }

    const auto attack = GetComponent<component::Attack>();
    if (!attack) { return; }

    input_state_.SetKeyState(key_event.GetKey(), key_event.IsPressed());

    Vector2 direction{ 0.0F, 0.0F };

    HandleDirectionInput(direction);
    GetComponent<component::Run>()->SetRunning(input_state_.IsKeyPressed(KEY_K));
    movement->SetDirection(direction);

    if (input_state_.IsKeyPressed(KEY_J) || input_state_.IsKeyPressed(KEY_L)) { HandleWeaponInput(); }

    attack->SetAttacking(input_state_.IsKeyPressed(KEY_I));


    break;
  }
  case event::Event::SoundEvent: {
    auto &sound_event = dynamic_cast<event::SoundEvent &>(event);
    if (const auto ab = audio::GetAudioBridge()) {
      ab->PlaySoundAt(sound_event.GetSoundName(),
        GetComponent<component::Transform>()->GetPosition().x,
        GetComponent<component::Transform>()->GetPosition().y,
        sound_event.GetVolume(),
        sound_event.GetPitch());
      sound_event.SetHandled(true);
    }
    break;
  }
  default:
    break;
  }
}

void Player::UpdateAttack(const float delta_time) const
{
  const auto attack = GetComponent<component::Attack>();
  if (!attack) { return; }

  const auto inventory = GetComponent<component::Inventory>();
  if (!inventory) { return; }

  const auto current_weapon = inventory->GetCurrentWeapon();
  if (!current_weapon) { return; }

  current_weapon->SetLastAttackTime(current_weapon->GetLastAttackTime() + delta_time);

  if (attack->IsAttacking()) {
    const float cooldown = current_weapon->GetCooldown();
    const float elapsed = current_weapon->GetLastAttackTime();
    if (elapsed >= cooldown) {
      HandleThrowInput(current_weapon);
      current_weapon->SetLastAttackTime(0.0F);
    }
  }
}

void Player::UpdateColliderSize() const
{
  const auto collider = GetComponent<component::ColliderBox>();

  auto new_size = Vector2{ 12.F, 28.F };
  auto new_offset = Vector2{ -6.F, -14.F };
  if (last_facing_ == FacingDir::Side) {
    new_size = Vector2{ 12.F, 12.F };
    new_offset = Vector2{ -6.F, 0.F };
  }

  collider->SetSize(new_size);
  collider->SetOffset(new_offset);
}

void Player::HandleThrowInput(const std::shared_ptr<component::Weapon> &weapon) const
{
  const auto transform = GetComponent<component::Transform>();
  if (!transform) { return; }

  Vector2 direction{ 0.0F, 0.0F };

  if (FacingDir::Up == last_facing_) {
    direction.y = -1.0F;
  } else if (FacingDir::Down == last_facing_) {
    direction.y = 1.0F;
  } else if (FacingDir::Side == last_facing_) {
    direction.x = last_left_ ? -1.0F : 1.0F;
  }

  if (weapon->GetWeaponType() == component::WeaponType::JAVELIN) {
    const auto projectile_event =
      std::make_shared<event::ProjectileEvent>(component::TypeProjectile::JAVELIN, direction, 80.0F);
    projectile_event->SetProjectileId(utils::UUID::Generate());
    projectile_event->SetPosition(transform->GetPosition());
    event::EventBus::Dispatch(*projectile_event);
  }
}

void Player::HandleDirectionInput(Vector2 &direction) const
{
  if (input_state_.IsKeyPressed(KEY_W)) { direction.y -= 1.0F; }
  if (input_state_.IsKeyPressed(KEY_S)) { direction.y += 1.0F; }
  if (input_state_.IsKeyPressed(KEY_A)) { direction.x -= 1.0F; }
  if (input_state_.IsKeyPressed(KEY_D)) { direction.x += 1.0F; }
}

void Player::HandleWeaponInput() const
{
  const auto inventory = GetComponent<component::Inventory>();
  if (!inventory) { return; }

  std::shared_ptr<component::Weapon> weapon(nullptr);

  if (input_state_.IsKeyPressed(KEY_J)) {
    weapon = inventory->ChangeToPreviousWeapon();
  } else {
    weapon = inventory->ChangeToNextWeapon();
  }
  SetCurrentWeapon(weapon);
}

void Player::SetCurrentWeapon(const std::shared_ptr<component::Weapon> &weapon) const
{
  if (!weapon) { return; }

  const auto anim = GetComponent<component::SpriteAnimation>();
  if (!anim) { return; }

  const auto inventory = GetComponent<component::Inventory>();
  if (!inventory) { return; }

  inventory->SetCurrentWeapon(weapon);
}

std::shared_ptr<GameObject> Player::Clone() { return std::make_shared<Player>(*this); }


}// namespace chroma::shared::core::player