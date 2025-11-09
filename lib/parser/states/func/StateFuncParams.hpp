#ifndef PARSER_STATEFUNCPARAMS_HPP_
#define PARSER_STATEFUNCPARAMS_HPP_

#include "lib/parser/states/base/StateBase.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

namespace ovum::compiler::parser {

class StateFuncParams : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) override;
};

} // namespace ovum::compiler::parser

#endif // PARSER_STATEFUNCPARAMS_HPP_
