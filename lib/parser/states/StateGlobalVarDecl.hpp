#ifndef STATEGLOBALVARDECL_HPP_
#define STATEGLOBALVARDECL_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateGlobalVarDecl : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEGLOBALVARDECL_HPP_
