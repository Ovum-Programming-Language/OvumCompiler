#ifndef STATEUNSAFEBLOCK_HPP_
#define STATEUNSAFEBLOCK_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateUnsafeBlock : public StateBase {
public:
  const char* Name() const override;
  bool Step(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEUNSAFEBLOCK_HPP_
