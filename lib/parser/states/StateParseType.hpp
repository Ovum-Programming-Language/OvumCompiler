#ifndef STATEPARSETYPE_HPP_
#define STATEPARSETYPE_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

class StateParseType : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) override;
};

#endif // STATEPARSETYPE_HPP_
