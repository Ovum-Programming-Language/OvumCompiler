#ifndef STATECLASSHDR_HPP_
#define STATECLASSHDR_HPP_

#include "../base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

class StateClassHdr : public StateBase {
public:
  const char* Name() const override;
  bool Step(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATECLASSHDR_HPP_
