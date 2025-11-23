#ifndef PARSER_STATEFIELDDECL_HPP_
#define PARSER_STATEFIELDDECL_HPP_

#include "base/StateBase.hpp"
#include "lib/parser/context/ContextParser.hpp"

namespace ovum::compiler::parser {

class StateFieldDecl : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) const override;
};

} // namespace ovum::compiler::parser

#endif // PARSER_STATEFIELDDECL_HPP_
