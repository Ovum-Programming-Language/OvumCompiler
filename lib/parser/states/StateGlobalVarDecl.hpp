#ifndef STATEGLOBALVARDECL_HPP_
#define STATEGLOBALVARDECL_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateGlobalVarDecl : public StateBase {
public:
  const char* Name() const override;
  bool Step(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEGLOBALVARDECL_HPP_
