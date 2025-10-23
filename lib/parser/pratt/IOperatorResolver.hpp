#ifndef IOPERATORRESOLVER_HPP_
#define IOPERATORRESOLVER_HPP_

#include "lib/parser/ast/nodes/exprs/tags/IUnaryOpTag.hpp"
#include "lib/parser/pratt/specifications/InfixSpec.hpp"
#include "lib/parser/pratt/specifications/PostfixSpec.hpp"

class IOperatorResolver {
public:
  virtual ~IOperatorResolver() = default;
  virtual const InfixSpec* FindInfix(const Token& t) const = 0;
  virtual const PostfixSpec* FindPostfix(const Token& t) const = 0;
  virtual const IUnaryOpTag* FindPrefix(const Token& t) const = 0;
  virtual bool IsContinuation(const Token& t) const = 0;
};

#endif // IOPERATORRESOLVER_HPP_
