#ifndef CHARHANDLER_HPP_
#define CHARHANDLER_HPP_

#include "Handler.hpp"
#include "lib/lexer/Lexer.hpp"

class CharHandler : public Handler {
public:
  OptToken Scan(Lexer& lx) override;
};

#endif // CHARHANDLER_HPP_
