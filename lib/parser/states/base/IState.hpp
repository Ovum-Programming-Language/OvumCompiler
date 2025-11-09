#ifndef PARSER_ISTATE_HPP_
#define PARSER_ISTATE_HPP_

#include <expected>

#include "StateError.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

namespace ovum::compiler::parser {

class ContextParser; // forward

class IState {
public:
  using StepResult = std::expected<bool, StateError>;

  virtual ~IState() = default;
  virtual std::string_view Name() const = 0;
  virtual StepResult TryStep(ContextParser& ctx, ITokenStream& ts) = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_ISTATE_HPP_
