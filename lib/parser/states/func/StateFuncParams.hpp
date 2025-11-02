#ifndef STATEFUNCPARAMS_HPP_
#define STATEFUNCPARAMS_HPP_

#include "lib/parser/states/base/StateBase.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

class StateFuncParams : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEFUNCPARAMS_HPP_
