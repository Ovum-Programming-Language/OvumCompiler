#ifndef ISTATE_HPP_
#define ISTATE_HPP_

#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

class IState {
public:
  virtual ~IState() = default;
  virtual const char* Name() const = 0;
  virtual bool Step(class ContextParser& ctx, ITokenStream& ts) = 0;
};

#endif // ISTATE_HPP_
