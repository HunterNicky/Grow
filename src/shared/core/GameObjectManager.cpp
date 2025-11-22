#include "chroma/shared/core/GameObjectManager.h"
#include <algorithm>
#include <mutex>

namespace chroma::shared::core {

GameObjectManager::GameObjectManager() : collision_manager_(nullptr) {}

void GameObjectManager::Register(const std::shared_ptr<GameObject> &object)
{
  if (!object) { return; }

  std::unique_lock const lock(mutex_);

  if (objects_map_.contains(object->GetId())) { return; }

  all_objects_.push_back(object);

  objects_map_[object->GetId()] = object;

  objects_by_type_[object->GetTag()].push_back(object);
  objects_by_layer_[object->GetLayer()].push_back(object);
}

void GameObjectManager::Unregister(const UUIDv4::UUID &id)
{
  std::unique_lock const lock(mutex_);

  const auto it = objects_map_.find(id);
  if (it == objects_map_.end()) { return; }

  const std::shared_ptr<GameObject> obj = it->second;

  RemoveFromVector(objects_by_type_[obj->GetTag()], id);
  RemoveFromVector(objects_by_layer_[obj->GetLayer()], id);

  RemoveFromVector(all_objects_, id);

  objects_map_.erase(it);
}

void GameObjectManager::RemoveFromVector(std::vector<std::shared_ptr<GameObject>> &vec, const UUIDv4::UUID &id)
{
  const auto it = std::ranges::find_if(vec, [&id](const auto &obj) { return obj->GetId() == id; });

  if (it != vec.end()) {
    std::iter_swap(it, vec.end() - 1);
    vec.pop_back();
  }
}

std::shared_ptr<collision::CollisionManager> GameObjectManager::GetCollisionManager() const
{
  return collision_manager_;
}

std::shared_ptr<GameObject> GameObjectManager::Get(const UUIDv4::UUID &id) const
{
  std::shared_lock const lock(mutex_);
  const auto it = objects_map_.find(id);
  return (it != objects_map_.end()) ? it->second : nullptr;
}

bool GameObjectManager::Exists(const UUIDv4::UUID &id) const
{
  std::shared_lock const lock(mutex_);
  return objects_map_.contains(id);
}

const std::vector<std::shared_ptr<GameObject>> &GameObjectManager::GetByTag(const GameObjectType tag) const
{
  std::shared_lock const lock(mutex_);
  const auto it = objects_by_type_.find(tag);
  if (it != objects_by_type_.end()) { return it->second; }
  return empty_list_;
}

const std::vector<std::shared_ptr<GameObject>> &GameObjectManager::GetByLayer(const uint32_t layer) const
{
  std::shared_lock const lock(mutex_);
  const auto it = objects_by_layer_.find(layer);
  if (it != objects_by_layer_.end()) { return it->second; }
  return empty_list_;
}

void GameObjectManager::Clear()
{
  std::unique_lock const lock(mutex_);
  all_objects_.clear();
  objects_map_.clear();
  objects_by_type_.clear();
  objects_by_layer_.clear();
}

void GameObjectManager::SetCollisionManager(const std::shared_ptr<collision::CollisionManager> &collision_manager)
{
  collision_manager_ = collision_manager;
}

void GameObjectManager::RefreshObject(const std::shared_ptr<GameObject> &object,
  const GameObjectType old_tag,
  const uint32_t old_layer)
{
  if (!object) { return; }
  std::unique_lock const lock(mutex_);

  RemoveFromVector(objects_by_type_[old_tag], object->GetId());
  RemoveFromVector(objects_by_layer_[old_layer], object->GetId());

  objects_by_type_[object->GetTag()].push_back(object);
  objects_by_layer_[object->GetLayer()].push_back(object);
}

std::unordered_map<UUIDv4::UUID, std::shared_ptr<GameObject>> GameObjectManager::CloneObjectsMap() const
{
  std::shared_lock const lock(mutex_);
  std::unordered_map<UUIDv4::UUID, std::shared_ptr<GameObject>> clone;

  for (const auto &[id, obj] : objects_map_) {
    if (obj) { clone[id] = obj->Clone(); }
  }

  return clone;
}

}// namespace chroma::shared::core