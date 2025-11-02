#ifndef STATEBASE_HPP_
#define STATEBASE_HPP_

#include "IState.hpp"
#include "lib/parser/tokens/SourceSpan.hpp"

class StateBase : IState {
public:
  using StepResult = StepResult;
  ~StateBase() override = default;
  static SourceSpan SpanFrom(const Token& token);
  static SourceSpan Union(const SourceSpan& lhs, const SourceSpan& rhs);
};

#endif // STATEBASE_HPP_
