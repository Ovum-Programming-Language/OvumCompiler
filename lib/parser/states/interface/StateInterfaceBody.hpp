#ifndef PARSER_STATEINTERFACEBODY_HPP_
#define PARSER_STATEINTERFACEBODY_HPP_

#include "lib/parser/states/base/StateBase.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

namespace ovum::compiler::parser {

class StateInterfaceBody : public StateBase {
public:
  std::string_view Name() const override;
  StepResult TryStep(ContextParser& ctx, ITokenStream& ts) const override;
};

} // namespace ovum::compiler::parser

#endif // PARSER_STATEINTERFACEBODY_HPP_
