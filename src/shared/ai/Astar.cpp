#include "chroma/shared/ai/Astar.h"

#include <queue>
#include <raymath.h>
#include <unordered_map>

namespace chroma::shared::ai {
// float Astar::Heuristic(const Vector2 a, const Vector2 b) { return std::abs(a.x - b.x) + std::abs(a.y - b.y); }
float Astar::Heuristic(const Vector2 a, const Vector2 b) { return Vector2Distance(a, b); }


std::vector<Vector2> Astar::Solve(const Vector2 start_pos,
  const Vector2 end_pos,
  const std::vector<NavNode> &grid,
  const int width,
  const int height)
{
  std::vector<Vector2> path;

  const int start_x = static_cast<int>(std::round(start_pos.x));
  const int start_y = static_cast<int>(std::round(start_pos.y));
  const int end_x = static_cast<int>(std::round(end_pos.x));
  const int end_y = static_cast<int>(std::round(end_pos.y));

  if (start_x < 0 || start_x >= width || start_y < 0 || start_y >= height || end_x < 0 || end_x >= width || end_y < 0
      || end_y >= height) {
    return path;
  }

  const int start_index = (start_y * width) + start_x;
  const int end_index = (end_y * width) + end_x;

  if (!grid[end_index].is_walkable) { return path; }

  if (start_index == end_index) { return path; }

  std::vector<NodeRecord> node_records(width * height);

  node_records[start_index].g = 0;
  node_records[start_index].h = Heuristic(start_pos, end_pos);
  node_records[start_index].f = node_records[start_index].h;
  node_records[start_index].open = true;

  std::priority_queue<int, std::vector<int>, CompareNode> open_list((CompareNode(node_records)));
  open_list.push(start_index);

  while (!open_list.empty()) {
    const int current_index = open_list.top();
    open_list.pop();

    if (node_records[current_index].closed) { continue; }

    node_records[current_index].closed = true;
    node_records[current_index].open = false;

    if (current_index == end_index) {
      int step = end_index;
      while (step != -1) {
        path.push_back(grid[step].position);
        step = node_records[step].parent_index;
      }
      std::ranges::reverse(path);
      return path;
    }

    const NavNode &current_node = grid[current_index];

    for (const Vector2 &neighbor_pos : current_node.neighbors) {
      const int nx = static_cast<int>(neighbor_pos.x);
      const int ny = static_cast<int>(neighbor_pos.y);
      int const neighbor_index = (ny * width) + nx;

      if (node_records[neighbor_index].closed) { continue; }
      const float tentative_g = node_records[current_index].g + 1.0F;

      if (tentative_g < node_records[neighbor_index].g) {
        node_records[neighbor_index].parent_index = current_index;
        node_records[neighbor_index].g = tentative_g;
        node_records[neighbor_index].h = Heuristic(neighbor_pos, end_pos);
        node_records[neighbor_index].f = node_records[neighbor_index].g + node_records[neighbor_index].h;

        if (!node_records[neighbor_index].open) {
          node_records[neighbor_index].open = true;
          open_list.push(neighbor_index);
        } else {
          open_list.push(neighbor_index);
        }
      }
    }
  }

  return {};
}

}// namespace chroma::shared::ai