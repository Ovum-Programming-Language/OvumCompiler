#ifndef STATEMODULE_HPP_
#define STATEMODULE_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateModule : public StateBase {
public:
  const char* Name() const override;
  bool Step(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEMODULE_HPP_
