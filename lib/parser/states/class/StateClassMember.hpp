#ifndef PARSER_STATECLASSMEMBER_HPP_
#define PARSER_STATECLASSMEMBER_HPP_

#include "lib/parser/states/base/StateBase.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

namespace ovum::compiler::parser {

class StateClassMember : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) const override;
};

} // namespace ovum::compiler::parser

#endif // PARSER_STATECLASSMEMBER_HPP_
