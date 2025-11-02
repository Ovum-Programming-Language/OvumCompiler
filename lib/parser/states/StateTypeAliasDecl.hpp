#ifndef STATETYPEALIASDECL_HPP_
#define STATETYPEALIASDECL_HPP_

#include "lib/parser/states/base/StateBase.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

class StateTypeAliasDecl : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATETYPEALIASDECL_HPP_
