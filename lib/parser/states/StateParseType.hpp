#ifndef PARSER_STATEPARSETYPE_HPP_
#define PARSER_STATEPARSETYPE_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

namespace ovum::compiler::parser {

class StateParseType : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) const override;
};

} // namespace ovum::compiler::parser

#endif // PARSER_STATEPARSETYPE_HPP_
