#ifndef RELATEDINFO_HPP_
#define RELATEDINFO_HPP_

#include <optional>
#include <string>

#include "lib/parser/tokens/SourceSpan.hpp"

class RelatedInfo {
public:
  // TODO: make methods
private:
  std::string message_;
  std::optional<SourceSpan> where_;
};

#endif // RELATEDINFO_HPP_
