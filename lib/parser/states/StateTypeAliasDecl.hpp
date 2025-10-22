#ifndef STATETYPEALIASDECL_HPP_
#define STATETYPEALIASDECL_HPP_

#include "lib/parser/states/base/StateBase.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

class StateTypeAliasDecl : public StateBase {
public:
  const char* Name() const override;
  bool Step(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATETYPEALIASDECL_HPP_
