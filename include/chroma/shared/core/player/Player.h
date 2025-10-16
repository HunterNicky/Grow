#pragma once

#include "chroma/shared/core/GameObject.h"

namespace chroma::shared::core::player{
    class Player : public GameObject {
    public:
        Player();
        void InitComponents();
        ~Player();
        void OnUpdate(float delta_time);
        void OnFixedUpdate(float fixed_delta_time);
        void OnCollision(const collision::CollisionEvent &event);
        void OnRender();    
    };
}