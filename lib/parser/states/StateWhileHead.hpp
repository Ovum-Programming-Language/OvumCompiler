#ifndef PARSER_STATEWHILEHEAD_HPP_
#define PARSER_STATEWHILEHEAD_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

namespace ovum::compiler::parser {

class StateWhileHead : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) const override;
};

} // namespace ovum::compiler::parser

#endif // PARSER_STATEWHILEHEAD_HPP_
