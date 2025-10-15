#ifndef NUMBERHANDLER_HPP_
#define NUMBERHANDLER_HPP_

#include <cmath>

#include "Handler.hpp"
#include "lib/lexer/Lexer.hpp"

class NumberHandler : public Handler {
public:
  OptToken Scan(Lexer &lx) override;
};

#endif // NUMBERHANDLER_HPP_
