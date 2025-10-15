#ifndef SLASHHANDLER_HPP_
#define SLASHHANDLER_HPP_

#include "Handler.hpp"
#include "lib/lexer/Lexer.hpp"

class SlashHandler : public Handler {
public:
  OptToken Scan(Lexer& lx) override;
};

#endif // SLASHHANDLER_HPP_
