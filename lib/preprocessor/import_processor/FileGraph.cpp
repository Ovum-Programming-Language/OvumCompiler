#include "FileGraph.hpp"

#include <algorithm>
#include <filesystem>
#include <queue>
#include <set>
#include <vector>

void FileGraph::AddDependency(const std::filesystem::path& from_path, const std::filesystem::path& to_path) {
  dependency_graph_[from_path].insert(to_path);
  nodes_.insert(from_path);
  nodes_.insert(to_path);
}

void FileGraph::Clear() {
  dependency_graph_.clear();
  nodes_.clear();
}

const std::map<std::filesystem::path, std::set<std::filesystem::path>>& FileGraph::GetDependencyGraph() const {
  return dependency_graph_;
}

bool FileGraph::DetectCycles(std::vector<std::filesystem::path>& cycle_path) const {
  std::map<std::filesystem::path, int32_t> node_colors;

  for (const std::filesystem::path& node : nodes_) {
    if (node_colors.find(node) == node_colors.end()) {
      if (DetectCycleDepthFirst(node, node_colors, cycle_path)) {
        return true;
      }
    }
  }

  return false;
}

bool FileGraph::DetectCycleDepthFirst(const std::filesystem::path& node,
                                      std::map<std::filesystem::path, int32_t>& node_colors,
                                      std::vector<std::filesystem::path>& cycle_path) const {
  node_colors[node] = 1;
  cycle_path.push_back(node);

  auto dependency_iterator = dependency_graph_.find(node);

  if (dependency_iterator != dependency_graph_.end()) {
    for (const std::filesystem::path& neighbor : dependency_iterator->second) {
      auto color_iterator = node_colors.find(neighbor);

      if (color_iterator == node_colors.end()) {
        if (DetectCycleDepthFirst(neighbor, node_colors, cycle_path)) {
          return true;
        }
      } else if (color_iterator->second == 1) {
        return true;
      }
    }
  }

  node_colors[node] = 2;
  cycle_path.pop_back();

  return false;
}

std::expected<std::vector<std::filesystem::path>, CycleDetectedError> FileGraph::TopologicalSort() const {
  std::map<std::filesystem::path, int32_t> in_degree;

  for (const std::filesystem::path& node : nodes_) {
    in_degree[node] = 0;
  }

  for (const auto& [_, dependencies] : dependency_graph_) {
    for (const std::filesystem::path& vertex_to : dependencies) {
      auto in_degree_iterator = in_degree.find(vertex_to);

      if (in_degree_iterator != in_degree.end()) {
        ++(in_degree_iterator->second);
      }
    }
  }

  std::queue<std::filesystem::path> nodes_queue;

  for (const auto& [node, in_degree_value] : in_degree) {
    if (in_degree_value == 0) {
      nodes_queue.push(node);
    }
  }

  std::vector<std::filesystem::path> topological_order;

  while (!nodes_queue.empty()) {
    std::filesystem::path current_node = nodes_queue.front();
    nodes_queue.pop();
    topological_order.push_back(current_node);
    auto dependencies_iterator = dependency_graph_.find(current_node);

    if (dependencies_iterator == dependency_graph_.end()) {
      continue;
    }

    for (const std::filesystem::path& adjacent_node : dependencies_iterator->second) {
      auto degree_iterator = in_degree.find(adjacent_node);

      if (degree_iterator != in_degree.end()) {
        --(degree_iterator->second);

        if (degree_iterator->second == 0) {
          nodes_queue.push(adjacent_node);
        }
      }
    }
  }

  if (topological_order.size() != nodes_.size()) {
    return std::unexpected(CycleDetectedError("Topological sort failed (cycle)"));
  }

  std::ranges::reverse(topological_order);
  return topological_order;
}
