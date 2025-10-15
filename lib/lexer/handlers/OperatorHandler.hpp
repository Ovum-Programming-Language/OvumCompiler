#ifndef OPERATORHANDLER_HPP_
#define OPERATORHANDLER_HPP_

#include "Handler.hpp"
#include "lib/lexer/Lexer.hpp"

class OperatorHandler : public Handler {
public:
  OptToken Scan(Lexer &lx) override;
};

#endif // OPERATORHANDLER_HPP_
