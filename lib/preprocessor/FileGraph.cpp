#include <algorithm>

#include "FileGraph.hpp"

void FileGraph::AddDependency(const std::filesystem::path& from, const std::filesystem::path& to) {
  dep_graph_[from].insert(to);
}

void FileGraph::Clear() {
  dep_graph_.clear();
}

const std::map<std::filesystem::path, std::set<std::filesystem::path>>& FileGraph::GetDepGraph()
    const {
  return dep_graph_;
}

bool FileGraph::DetectCycles(const std::set<std::filesystem::path>& nodes,
                             std::vector<std::filesystem::path>& cycle_path) const {
  std::map<std::filesystem::path, int> colors;
  for (const auto& node : nodes) {
    if (colors.find(node) == colors.end()) {
      if (DfsDetect(node, colors, cycle_path)) {
        return true;
      }
    }
  }
  return false;
}

bool FileGraph::DfsDetect(const std::filesystem::path& node,
                          std::map<std::filesystem::path, int>& colors,
                          std::vector<std::filesystem::path>& cycle_path) const {
  colors[node] = 1;
  cycle_path.push_back(node);

  auto it = dep_graph_.find(node);
  if (it != dep_graph_.end()) {
    for (const auto& neighbor : it->second) {
      auto color_it = colors.find(neighbor);
      if (color_it == colors.end()) {
        if (DfsDetect(neighbor, colors, cycle_path)) {
          return true;
        }
      } else if (color_it->second == 1) {
        return true;
      }
    }
  }

  colors[node] = 2;
  cycle_path.pop_back();

  return false;
}

std::expected<std::vector<std::filesystem::path>, CycleDetectedError> FileGraph::TopologicalSort(
    const std::set<std::filesystem::path>& nodes) const {
  if (nodes.empty()) {
    return std::vector<std::filesystem::path>{};
  }

  std::map<std::filesystem::path, int> in_degree;
  for (const auto& u : nodes) {
    in_degree[u] = 0;
  }

  for (const auto& [u, deps] : dep_graph_) {
    for (const std::filesystem::path& v : deps) {
      auto it = in_degree.find(v);
      if (it != in_degree.end()) {
        ++(it->second);
      }
    }
  }

  std::queue<std::filesystem::path> q;
  for (const auto& [node, deg] : in_degree) {
    if (deg == 0) {
      q.push(node);
    }
  }

  std::vector<std::filesystem::path> order;
  while (!q.empty()) {
    std::filesystem::path u = q.front();
    q.pop();
    order.push_back(u);

    auto deps_it = dep_graph_.find(u);
    if (deps_it != dep_graph_.end()) {
      for (const std::filesystem::path& v : deps_it->second) {
        auto deg_it = in_degree.find(v);
        if (deg_it != in_degree.end()) {
          --(deg_it->second);
          if (deg_it->second == 0) {
            q.push(v);
          }
        }
      }
    }
  }

  if (order.size() != nodes.size()) {
    return std::unexpected(CycleDetectedError("Topological sort failed (cycle)"));
  }

  std::ranges::reverse(order);
  return order;
}
