#ifndef STATEVARDECLTAIL_HPP_
#define STATEVARDECLTAIL_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateVarDeclTail : public StateBase {
public:
  const char* Name() const override;
  bool Step(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEVARDECLTAIL_HPP_
