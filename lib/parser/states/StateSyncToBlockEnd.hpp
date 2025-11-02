#ifndef STATESYNCTOBLOCKEND_HPP_
#define STATESYNCTOBLOCKEND_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateSyncToBlockEnd : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATESYNCTOBLOCKEND_HPP_
