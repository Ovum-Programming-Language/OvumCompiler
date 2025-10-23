#ifndef STATESYNCTOBLOCKEND_HPP_
#define STATESYNCTOBLOCKEND_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateSyncToBlockEnd : public StateBase {
public:
  const char* Name() const override;
  bool Step(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATESYNCTOBLOCKEND_HPP_
