#ifndef STATEFUNCBODY_HPP_
#define STATEFUNCBODY_HPP_

#include "lib/parser/states/base/StateBase.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

class StateFuncBody : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEFUNCBODY_HPP_
