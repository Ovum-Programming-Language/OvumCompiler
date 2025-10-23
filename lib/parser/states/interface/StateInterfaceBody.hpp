#ifndef STATEINTERFACEBODY_HPP_
#define STATEINTERFACEBODY_HPP_

#include "lib/parser/states/base/StateBase.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

class StateInterfaceBody : public StateBase {
public:
  const char* Name() const override;
  bool Step(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEINTERFACEBODY_HPP_
