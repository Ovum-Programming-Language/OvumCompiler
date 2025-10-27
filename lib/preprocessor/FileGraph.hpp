#ifndef FILEGRAPH_HPP_
#define FILEGRAPH_HPP_

#include <expected>
#include <filesystem>
#include <queue>
#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PreprocessorError.hpp"

class FileGraph {
public:
  void AddDependency(const std::filesystem::path& from, const std::filesystem::path& to);
  void Clear();

  [[nodiscard]] bool DetectCycles(const std::unordered_set<std::filesystem::path>& nodes,
                                  std::vector<std::filesystem::path>& cycle_path) const;
  [[nodiscard]] std::expected<std::vector<std::filesystem::path>, CycleDetectedError> TopologicalSort(
      const std::unordered_set<std::filesystem::path>& nodes) const;
  [[nodiscard]] const std::unordered_map<std::filesystem::path, std::unordered_set<std::filesystem::path>>&
  GetDepGraph() const;

private:
  std::unordered_map<std::filesystem::path, std::unordered_set<std::filesystem::path>> dep_graph_;

  bool DfsDetect(const std::filesystem::path& node,
                 std::unordered_map<std::filesystem::path, int>& colors,
                 std::vector<std::filesystem::path>& cycle_path) const;
};

#endif // FILEGRAPH_HPP_
