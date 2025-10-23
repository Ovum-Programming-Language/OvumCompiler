#ifndef STATECALLDECLHDR_HPP_
#define STATECALLDECLHDR_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateCallDeclHdr : public StateBase {
public:
  const char* Name() const override;
  bool Step(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATECALLDECLHDR_HPP_
