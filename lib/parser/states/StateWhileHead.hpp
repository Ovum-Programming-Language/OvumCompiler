#ifndef STATEWHILEHEAD_HPP_
#define STATEWHILEHEAD_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateWhileHead : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEWHILEHEAD_HPP_
