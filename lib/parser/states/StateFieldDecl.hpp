#ifndef STATEFIELDDECL_HPP_
#define STATEFIELDDECL_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateFieldDecl : public StateBase {
public:
  const char* Name() const override;
  bool Step(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEFIELDDECL_HPP_
