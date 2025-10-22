#ifndef STATESYNCTOSTMTEND_HPP_
#define STATESYNCTOSTMTEND_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateSyncToStmtEnd : public StateBase {
public:
  const char* Name() const override;
  bool Step(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATESYNCTOSTMTEND_HPP_
