#ifndef DEFAULTOPERATORRESOLVER_HPP_
#define DEFAULTOPERATORRESOLVER_HPP_

#include <vector>

#include "IOperatorResolver.hpp"
#include "lib/parser/ast/nodes/exprs/tags/IUnaryOpTag.hpp"
#include "specifications/InfixSpec.hpp"
#include "specifications/PostfixSpec.hpp"

class DefaultOperatorResolver : public IOperatorResolver {
public:
  ~DefaultOperatorResolver() override = default;

  const InfixSpec* FindInfix(const Token& t) const override;
  const PostfixSpec* FindPostfix(const Token& t) const override;
  const IUnaryOpTag* FindPrefix(const Token& t) const override;
  bool IsContinuation(const Token& t) const override;

private:
  std::vector<InfixSpec> infix_;
  std::vector<PostfixSpec> postfix_;
};

#endif // DEFAULTOPERATORRESOLVER_HPP_
