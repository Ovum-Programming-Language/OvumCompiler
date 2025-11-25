#ifndef PARSER_DEFAULTOPERATORRESOLVER_HPP_
#define PARSER_DEFAULTOPERATORRESOLVER_HPP_

#include <functional>
#include <optional>
#include <vector>

#include "IOperatorResolver.hpp"
#include "lib/parser/ast/nodes/exprs/tags/IUnaryOpTag.hpp"
#include "specifications/InfixSpec.hpp"
#include "specifications/PostfixSpec.hpp"
#include "tokens/Token.hpp"

namespace ovum::compiler::parser {

class DefaultOperatorResolver : public IOperatorResolver {
public:
  ~DefaultOperatorResolver() override = default;

  std::optional<std::reference_wrapper<const InfixSpec>> FindInfix(const Token& t) const override;
  std::optional<std::reference_wrapper<const PostfixSpec>> FindPostfix(const Token& t) const override;
  std::optional<std::reference_wrapper<const IUnaryOpTag>> FindPrefix(const Token& t) const override;
  bool IsContinuation(const Token& t) const override;

private:
  std::vector<InfixSpec> infix_;
  std::vector<PostfixSpec> postfix_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_DEFAULTOPERATORRESOLVER_HPP_
