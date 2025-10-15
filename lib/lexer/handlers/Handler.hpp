#ifndef HANDLER_HPP_
#define HANDLER_HPP_

#include <memory>
#include <optional>

class Token;
class Lexer;

using TokenPtr = std::unique_ptr<Token>;
using OptToken = std::optional<TokenPtr>;

class Handler {
public:
  virtual ~Handler() = default;
  virtual OptToken Scan(Lexer &lx) = 0;
};

#endif // HANDLER_HPP_
