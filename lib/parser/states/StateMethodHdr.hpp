#ifndef PARSER_STATEMETHODHDR_HPP_
#define PARSER_STATEMETHODHDR_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

namespace ovum::compiler::parser {

class StateMethodHdr : public StateBase {
public:
  [[nodiscard]] std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) const override;
};

} // namespace ovum::compiler::parser

#endif // PARSER_STATEMETHODHDR_HPP_
