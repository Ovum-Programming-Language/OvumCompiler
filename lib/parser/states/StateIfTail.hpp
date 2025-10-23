#ifndef STATEIFTAIL_HPP_
#define STATEIFTAIL_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateIfTail : public StateBase {
public:
  const char* Name() const override;
  bool Step(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEIFTAIL_HPP_
