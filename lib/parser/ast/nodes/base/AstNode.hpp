#ifndef PARSER_ASTNODE_HPP_
#define PARSER_ASTNODE_HPP_

#include "lib/parser/tokens/SourceSpan.hpp"

namespace ovum::compiler::parser {

class AstVisitor; // forward

class AstNode { // NOLINT(cppcoreguidelines-special-member-functions)
public:
  virtual ~AstNode() = default;

  [[nodiscard]] const SourceSpan& Span() const noexcept;
  void SetSpan(SourceSpan span);
  void SetSpanParts(SourceId id, TokenPosition begin, TokenPosition end);
  void UnionSpan(const SourceSpan& other);

  virtual void Accept(AstVisitor& visitor) = 0;

private:
  SourceSpan span_{};
};

} // namespace ovum::compiler::parser

#endif // PARSER_ASTNODE_HPP_
