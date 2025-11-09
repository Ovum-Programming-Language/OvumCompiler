#ifndef PARSER_STATEUNSAFEBLOCK_HPP_
#define PARSER_STATEUNSAFEBLOCK_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

namespace ovum::compiler::parser {

class StateUnsafeBlock : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) override;
};

} // namespace ovum::compiler::parser

#endif // PARSER_STATEUNSAFEBLOCK_HPP_
