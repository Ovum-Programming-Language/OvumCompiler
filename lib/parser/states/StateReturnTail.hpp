#ifndef STATERETURNTAIL_HPP_
#define STATERETURNTAIL_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateReturnTail : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATERETURNTAIL_HPP_
