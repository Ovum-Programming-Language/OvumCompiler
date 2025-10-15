#ifndef WHITESPACEHANDLER_HPP_
#define WHITESPACEHANDLER_HPP_

#include "Handler.hpp"
#include "lib/lexer/Lexer.hpp"

class WhitespaceHandler : public Handler {
public:
  OptToken Scan(Lexer& lx) override;
};

#endif // WHITESPACEHANDLER_HPP_
