#ifndef PARSER_IOPERATORRESOLVER_HPP_
#define PARSER_IOPERATORRESOLVER_HPP_

#include <functional>
#include <optional>

#include "lib/parser/ast/nodes/exprs/tags/IUnaryOpTag.hpp"
#include "specifications/InfixSpec.hpp"
#include "specifications/PostfixSpec.hpp"

namespace ovum::compiler::parser {

class IOperatorResolver { // NOLINT(cppcoreguidelines-special-member-functions)
public:
  virtual ~IOperatorResolver() = default;

  [[nodiscard]] virtual std::optional<std::reference_wrapper<const InfixSpec>> FindInfix(const Token& t) const = 0;
  [[nodiscard]] virtual std::optional<std::reference_wrapper<const PostfixSpec>> FindPostfix(const Token& t) const = 0;
  [[nodiscard]] virtual std::optional<std::reference_wrapper<const IUnaryOpTag>> FindPrefix(const Token& t) const = 0;
  [[nodiscard]] virtual bool IsContinuation(const Token& t) const = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_IOPERATORRESOLVER_HPP_
