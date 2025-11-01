#ifndef FILEGRAPH_HPP_
#define FILEGRAPH_HPP_

#include <cstdint>
#include <expected>
#include <filesystem>
#include <map>
#include <set>
#include <vector>

#include "lib/preprocessor/PreprocessorError.hpp"

class FileGraph {
public:
  void AddDependency(const std::filesystem::path& from_path, const std::filesystem::path& to_path);
  void Clear();

  [[nodiscard]] bool DetectCycles(std::vector<std::filesystem::path>& cycle_path) const;
  [[nodiscard]] std::expected<std::vector<std::filesystem::path>, CycleDetectedError> TopologicalSort() const;
  [[nodiscard]] const std::map<std::filesystem::path, std::set<std::filesystem::path>>& GetDependencyGraph() const;

private:
  std::map<std::filesystem::path, std::set<std::filesystem::path>> dependency_graph_;
  std::set<std::filesystem::path> nodes_;

  bool DetectCycleDepthFirst(const std::filesystem::path& node,
                             std::map<std::filesystem::path, int32_t>& node_colors,
                             std::vector<std::filesystem::path>& cycle_path) const;
};

#endif // FILEGRAPH_HPP_
