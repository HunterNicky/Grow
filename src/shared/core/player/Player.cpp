#include "chroma/shared/core/player/Player.h"

#include "chroma/shared/core/components/Speed.h"
#include "chroma/shared/core/components/Transform.h"
#include "chroma/shared/core/GameObject.h"
#include "raylib.h" 
#include <memory>
#include <cmath>

namespace chroma::shared::core::player
{
    Player::Player()
    {
        Type_ = GameObjectType::PLAYER;
    }

    void Player::InitComponents()
    {
        auto speed_component = std::make_shared<component::Speed>(200.0F);
        AttachComponent(speed_component);
        transform_->SetScale({50.0F, 50.0F});
        AttachComponent(transform_);
    }

    Player::~Player() = default;

    void Player::OnUpdate(float delta_time)
    {
        auto transform = GetComponent<component::Transform>();
        auto speed = GetComponent<component::Speed>();
        if (!transform || !speed) 
        {
            return;
        }

        Vector2 direction = { 0.0F, 0.0F };

        if (IsKeyDown(KEY_W)) { direction.y -= 1.0F; }
        if (IsKeyDown(KEY_S)) { direction.y += 1.0F; }
        if (IsKeyDown(KEY_A)) { direction.x -= 1.0F; }
        if (IsKeyDown(KEY_D)) { direction.x += 1.0F; }
        
        const float magnitude = std::sqrt((direction.x * direction.x) + (direction.y * direction.y));
        
        if (magnitude > 0.0F) {
            direction.x /= magnitude;
            direction.y /= magnitude;
        }

        Vector2 pos = transform->GetPosition();
        pos.x += direction.x * speed->GetSpeed().x * delta_time;
        pos.y += direction.y * speed->GetSpeed().y * delta_time;
        transform->SetPosition(pos);
    }

    void Player::OnFixedUpdate(float fixed_delta_time) {
        (void)fixed_delta_time;
    }

    void Player::OnCollision(const collision::CollisionEvent &event) {
        (void)event;
    }

    void Player::OnRender() 
    {
        auto transform = GetComponent<component::Transform>();
        if (!transform) { return; }

        DrawRectangleV(transform->GetPosition(), transform->GetScale(), BLUE);
    }
}
