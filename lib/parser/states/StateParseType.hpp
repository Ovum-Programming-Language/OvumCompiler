#ifndef STATEPARSETYPE_HPP_
#define STATEPARSETYPE_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateParseType : public StateBase {
public:
  const char* Name() const override;
  bool Step(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEPARSETYPE_HPP_
