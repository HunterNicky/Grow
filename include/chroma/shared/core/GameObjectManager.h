#pragma once

#include "chroma/shared/collision/CollisionManager.h"
#include "chroma/shared/core/GameObject.h"

#include <memory>
#include <shared_mutex>
#include <unordered_map>
#include <uuid_v4.h>
#include <vector>

namespace chroma::shared::core {

class GameObjectManager
{
public:
  GameObjectManager();
  ~GameObjectManager() = default;

  GameObjectManager(const GameObjectManager &) = delete;
  GameObjectManager &operator=(const GameObjectManager &) = delete;
  GameObjectManager(GameObjectManager &&) = delete;
  GameObjectManager &operator=(GameObjectManager &&) = delete;

  void Register(const std::shared_ptr<GameObject> &object);
  void Unregister(const UUIDv4::UUID &id);
  void Clear();

  void SetCollisionManager(const std::shared_ptr<collision::CollisionManager> &collision_manager);
  std::shared_ptr<collision::CollisionManager> GetCollisionManager() const;

  std::shared_ptr<GameObject> Get(const UUIDv4::UUID &id) const;
  bool Exists(const UUIDv4::UUID &id) const;
  const std::vector<std::shared_ptr<GameObject>> &GetByTag(GameObjectType tag) const;
  const std::vector<std::shared_ptr<GameObject>> &GetByLayer(uint32_t layer) const;

  template<typename Func> void ForEach(Func &&func) const
  {
    std::vector<std::shared_ptr<GameObject>> snapshot;
    {
      std::shared_lock const lock(mutex_);
      snapshot = all_objects_;
    }

    for (const auto &object : snapshot) {
      if (object) { func(object); }
    }
  }

  template<typename Predicate> std::vector<std::shared_ptr<GameObject>> Query(Predicate &&predicate) const
  {
    std::shared_lock const lock(mutex_);
    std::vector<std::shared_ptr<GameObject>> results;
    if (!all_objects_.empty()) {}
    std::copy_if(all_objects_.begin(),
      all_objects_.end(),
      std::back_inserter(results),
      [&predicate](const std::shared_ptr<GameObject> &object) { return object && predicate(object); });
    return results;
  }

  template<typename Predicate> std::shared_ptr<GameObject> FindFirst(Predicate &&predicate) const
  {
    std::shared_lock const lock(mutex_);
    auto it = std::find_if(all_objects_.begin(),
      all_objects_.end(),
      [&predicate](const std::shared_ptr<GameObject> &object) { return object && predicate(object); });

    return nullptr;
  }

  void RefreshObject(const std::shared_ptr<GameObject> &object, GameObjectType old_tag, uint32_t old_layer);

  std::unordered_map<UUIDv4::UUID, std::shared_ptr<GameObject>> CloneObjectsMap() const;

private:
  std::shared_ptr<collision::CollisionManager> collision_manager_;

  static void RemoveFromVector(std::vector<std::shared_ptr<GameObject>> &vec, const UUIDv4::UUID &id);

  mutable std::shared_mutex mutex_;

  std::vector<std::shared_ptr<GameObject>> all_objects_;

  std::unordered_map<UUIDv4::UUID, std::shared_ptr<GameObject>> objects_map_;
  std::unordered_map<GameObjectType, std::vector<std::shared_ptr<GameObject>>> objects_by_type_;
  std::unordered_map<uint32_t, std::vector<std::shared_ptr<GameObject>>> objects_by_layer_;

  const std::vector<std::shared_ptr<GameObject>> empty_list_;
};

}// namespace chroma::shared::core