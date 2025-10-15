#ifndef IDENTIFIERHANDLER_HPP_
#define IDENTIFIERHANDLER_HPP_

#include "Handler.hpp"
#include "lib/lexer/Lexer.hpp"

class IdentifierHandler : public Handler {
public:
  OptToken Scan(Lexer &lx) override;
};

#endif // IDENTIFIERHANDLER_HPP_
