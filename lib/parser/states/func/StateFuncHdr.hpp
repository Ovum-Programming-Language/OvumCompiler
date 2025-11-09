#ifndef PARSER_STATEFUNCHDR_HPP_
#define PARSER_STATEFUNCHDR_HPP_

#include "lib/parser/states/base/StateBase.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

namespace ovum::compiler::parser {

class StateFuncHdr : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) override;
};

} // namespace ovum::compiler::parser

#endif // PARSER_STATEFUNCHDR_HPP_
