#ifndef STATEINTERFACEDECL_HPP_
#define STATEINTERFACEDECL_HPP_

#include "lib/parser/states/base/StateBase.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

class StateInterfaceDecl : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEINTERFACEDECL_HPP_
