#ifndef STATEMETHODHDR_HPP_
#define STATEMETHODHDR_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateMethodHdr : public StateBase {
public:
  const char* Name() const override;
  bool Step(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEMETHODHDR_HPP_
