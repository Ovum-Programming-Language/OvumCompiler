#ifndef STATESYNCTOSTMTEND_HPP_
#define STATESYNCTOSTMTEND_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateSyncToStmtEnd : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATESYNCTOSTMTEND_HPP_
