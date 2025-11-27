#pragma once

#include <algorithm>
#include <cmath>
#include <raylib.h>
#include <vector>

namespace chroma::shared::ai {
struct NavNode
{
  Vector2 position;
  bool is_walkable = false;
  std::vector<Vector2> neighbors;
};

struct NodeRecord
{
  int parent_index = -1;
  float g = std::numeric_limits<float>::max();
  float h = 0.0F;
  float f = std::numeric_limits<float>::max();
  bool closed = false;
  bool open = false;
};

struct CompareNode
{
  const std::vector<NodeRecord> &records;

  explicit CompareNode(const std::vector<NodeRecord> &r) : records(r) {}

  bool operator()(const int a, const int b) const { return records[a].f > records[b].f; }
};

class Astar
{
public:
  static std::vector<Vector2>
    Solve(Vector2 start_pos, Vector2 end_pos, const std::vector<NavNode> &grid, int width, int height);

private:
  static float Heuristic(Vector2 a, Vector2 b);
};

}// namespace chroma::shared::ai