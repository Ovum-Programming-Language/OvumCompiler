#ifndef STATEMETHODHDR_HPP_
#define STATEMETHODHDR_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateMethodHdr : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEMETHODHDR_HPP_
