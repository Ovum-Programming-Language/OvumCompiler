#ifndef STATEWHILEHEAD_HPP_
#define STATEWHILEHEAD_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateWhileHead : public StateBase {
public:
  const char* Name() const override;
  bool Step(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEWHILEHEAD_HPP_
