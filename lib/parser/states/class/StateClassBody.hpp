#ifndef STATECLASSBODY_HPP_
#define STATECLASSBODY_HPP_

#include "../base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

class StateClassBody : public StateBase {
public:
  const char* Name() const override;
  bool Step(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATECLASSBODY_HPP_
