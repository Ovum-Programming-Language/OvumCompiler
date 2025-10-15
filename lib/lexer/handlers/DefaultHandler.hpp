#ifndef DEFAULTHANDLER_HPP_
#define DEFAULTHANDLER_HPP_

#include "Handler.hpp"
#include "lib/lexer/Lexer.hpp"

class DefaultHandler : public Handler {
public:
  OptToken Scan(Lexer &lx) override;
};

#endif // DEFAULTHANDLER_HPP_
