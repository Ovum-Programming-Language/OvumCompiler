#ifndef STATEBLOCK_HPP_
#define STATEBLOCK_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateBlock : public StateBase {
public:
  const char* Name() const override;
  bool Step(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEBLOCK_HPP_
