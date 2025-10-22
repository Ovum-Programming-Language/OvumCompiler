#ifndef STATEFUNCPARAMS_HPP_
#define STATEFUNCPARAMS_HPP_

#include "lib/parser/states/base/StateBase.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

class StateFuncParams : public StateBase {
public:
  const char* Name() const override;
  bool Step(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEFUNCPARAMS_HPP_
