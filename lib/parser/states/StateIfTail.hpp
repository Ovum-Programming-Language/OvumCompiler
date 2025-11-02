#ifndef STATEIFTAIL_HPP_
#define STATEIFTAIL_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateIfTail : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEIFTAIL_HPP_
