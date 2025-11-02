#ifndef STATECLASSHDR_HPP_
#define STATECLASSHDR_HPP_

#include "lib/parser/states/base/StateBase.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

class StateClassHdr : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATECLASSHDR_HPP_
