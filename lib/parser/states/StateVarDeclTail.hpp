#ifndef STATEVARDECLTAIL_HPP_
#define STATEVARDECLTAIL_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateVarDeclTail : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEVARDECLTAIL_HPP_
