#ifndef STATERETURNTAIL_HPP_
#define STATERETURNTAIL_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateReturnTail : public StateBase {
public:
  const char* Name() const override;
  bool Step(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATERETURNTAIL_HPP_
