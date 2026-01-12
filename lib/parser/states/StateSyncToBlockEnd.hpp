#ifndef PARSER_STATESYNCTOBLOCKEND_HPP_
#define PARSER_STATESYNCTOBLOCKEND_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

namespace ovum::compiler::parser {

class StateSyncToBlockEnd : public StateBase {
public:
  [[nodiscard]] std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) const override;
};

} // namespace ovum::compiler::parser

#endif // PARSER_STATESYNCTOBLOCKEND_HPP_
