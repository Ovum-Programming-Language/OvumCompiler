#ifndef PARSER_IOPERATORRESOLVER_HPP_
#define PARSER_IOPERATORRESOLVER_HPP_

#include <functional>
#include <optional>

namespace ovum::compiler::parser {

class Token;
class IUnaryOpTag;
class InfixSpec;
class PostfixSpec;

class IOperatorResolver {
public:
  virtual ~IOperatorResolver() = default;

  virtual std::optional<std::reference_wrapper<const InfixSpec>> FindInfix(const Token& t) const = 0;
  virtual std::optional<std::reference_wrapper<const PostfixSpec>> FindPostfix(const Token& t) const = 0;
  virtual std::optional<std::reference_wrapper<const IUnaryOpTag>> FindPrefix(const Token& t) const = 0;
  virtual bool IsContinuation(const Token& t) const = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_IOPERATORRESOLVER_HPP_
