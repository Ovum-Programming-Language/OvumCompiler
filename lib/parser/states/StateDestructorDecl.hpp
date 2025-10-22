#ifndef STATEDESTRUCTORDECL_HPP_
#define STATEDESTRUCTORDECL_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateDestructorDecl : public StateBase {
public:
  const char* Name() const override;
  bool Step(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEDESTRUCTORDECL_HPP_
