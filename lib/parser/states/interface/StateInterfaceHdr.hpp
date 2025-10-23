#ifndef STATEINTERFACEHDR_HPP_
#define STATEINTERFACEHDR_HPP_

#include "lib/parser/states/base/StateBase.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

class StateInterfaceHdr : public StateBase {
public:
  const char* Name() const override;
  bool Step(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEINTERFACEHDR_HPP_
