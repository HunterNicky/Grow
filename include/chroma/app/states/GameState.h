#pragma once 

#include "chroma/app/states/State.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/app/states/mediator/GameNetworkMediator.h"

#include <memory>
#include <unordered_map>

enum SelectLevel : uint8_t {
    MeadowLevel = 0,
};

namespace chroma::app::states {
class GameState : public State
{
public:
    GameState();
    explicit GameState(std::shared_ptr<chroma::app::states::GameNetworkMediator> network_mediator);

    void OnRender() override;
    void OnUpdate(float delta_time) override;

    void SetGameObjects(const std::unordered_map<UUIDv4::UUID, std::shared_ptr<chroma::shared::core::GameObject>>& game_objects);
    [[nodiscard]] const std::unordered_map<UUIDv4::UUID, std::shared_ptr<chroma::shared::core::GameObject>>& GetGameObjects() const;

private:
    std::unordered_map<UUIDv4::UUID, std::shared_ptr<chroma::shared::core::GameObject>> game_objects_;
    std::shared_ptr<chroma::app::states::GameNetworkMediator> network_mediator_;
};
} // namespace chroma::app::states
