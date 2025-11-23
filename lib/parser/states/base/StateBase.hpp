#ifndef PARSER_STATEBASE_HPP_
#define PARSER_STATEBASE_HPP_

#include "IState.hpp"
#include "lib/parser/tokens/SourceSpan.hpp"

namespace ovum::compiler::parser {

class StateBase : public IState {
public:
  using StepResult = IState::StepResult;
  ~StateBase() override = default;

  static SourceSpan SpanFrom(const Token& token);
  static SourceSpan Union(const SourceSpan& lhs, const SourceSpan& rhs);
};

} // namespace ovum::compiler::parser

#endif // PARSER_STATEBASE_HPP_
