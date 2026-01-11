#ifndef PARSER_STATEMODULE_HPP_
#define PARSER_STATEMODULE_HPP_

#include "lib/parser/states/base/StateBase.hpp"

namespace ovum::compiler::parser {

class StateModule : public StateBase {
public:
  [[nodiscard]] std::string_view Name() const override;
  StepResult TryStep(ContextParser& context, ITokenStream& token_stream) const override;
};

} // namespace ovum::compiler::parser

#endif // PARSER_STATEMODULE_HPP_
