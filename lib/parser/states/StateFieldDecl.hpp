#ifndef STATEFIELDDECL_HPP_
#define STATEFIELDDECL_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateFieldDecl : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEFIELDDECL_HPP_
