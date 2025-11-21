#pragma once 

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Component.h"
#include "chroma/shared/core/components/Inventory.h"
#include "components_generated.h"

namespace chroma::shared::packet::adapter {
class ComponentAdapter
{
public:
    static void ToComponent(const std::shared_ptr<core::GameObject> &game_object,
      flatbuffers::FlatBufferBuilder &builder,
      std::vector<flatbuffers::Offset<Game::Component>> &fb_components);
    static void FromComponent(const Game::Component &component, std::shared_ptr<core::GameObject> &game_objec);

private:

    //
    // ============================================================
    //  FLATBUFFER COMPONENT --> GAME OBJECT COMPONENT
    // ============================================================
    //
    static void ComponentToSpeed(const Game::Component* component, const std::shared_ptr<core::GameObject>& game_object);
    static void ComponentToTransform(const Game::Component* component, const std::shared_ptr<core::GameObject>& game_object);
    static void ComponentToMovement(const Game::Component* component, const std::shared_ptr<core::GameObject>& game_object);
    static void ComponentToColor(const Game::Component* component, const std::shared_ptr<core::GameObject>& game_object);
    static void ComponentToHealth(const Game::Component* component, const std::shared_ptr<core::GameObject>& game_object);
    static void ComponentToRun(const Game::Component* component, const std::shared_ptr<core::GameObject>& game_object);
    static void ComponentToInventory(const Game::Component* component, const std::shared_ptr<core::GameObject>& game_object);

    //
    // ===========================================================
    // FLATBUFFER COMPONENT --> GAME OBJECT COMPONENT HELPERS
    // ===========================================================
    //
    static void ComponentToItem(const Game::Item* item, const std::shared_ptr<core::component::Inventory>& inventory);
    static void ItemToComponent(const std::shared_ptr<core::component::Item>& item,
      flatbuffers::FlatBufferBuilder &builder, std::vector<flatbuffers::Offset<Game::Item>> &fb_items);
   
    //
    // ============================================================
    //  GAME OBJECT COMPONENT --> FLATBUFFER COMPONENT
    // ============================================================
    //
    static void SpeedToComponent(const std::shared_ptr<core::component::Component> &component,
      flatbuffers::FlatBufferBuilder &builder, std::vector<flatbuffers::Offset<Game::Component>> &fb_components);
    static void TransformToComponent(const std::shared_ptr<core::component::Component> &component,
        flatbuffers::FlatBufferBuilder &builder, std::vector<flatbuffers::Offset<Game::Component>> &fb_components);
    static void MovementToComponent(const std::shared_ptr<core::component::Component> &component,
        flatbuffers::FlatBufferBuilder &builder, std::vector<flatbuffers::Offset<Game::Component>> &fb_components);
    static void ColorToComponent(const std::shared_ptr<core::component::Component> &component,
        flatbuffers::FlatBufferBuilder &builder, std::vector<flatbuffers::Offset<Game::Component>> &fb_components);
    static void HealthToComponent(const std::shared_ptr<core::component::Component> &component,
        flatbuffers::FlatBufferBuilder &builder, std::vector<flatbuffers::Offset<Game::Component>> &fb_components);
    static void RunToComponent(const std::shared_ptr<core::component::Component> &component,
        flatbuffers::FlatBufferBuilder &builder, std::vector<flatbuffers::Offset<Game::Component>> &fb_components);
    static void InventoryToComponent( const std::shared_ptr<core::component::Component> &component,
    flatbuffers::FlatBufferBuilder &builder, std::vector<flatbuffers::Offset<Game::Component>> &fb_components);
    
};
} // namespace chroma::shared::packet::adapter