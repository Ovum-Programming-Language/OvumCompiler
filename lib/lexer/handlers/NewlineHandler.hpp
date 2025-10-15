#ifndef NEWLINEHANDLER_HPP_
#define NEWLINEHANDLER_HPP_

#include "Handler.hpp"
#include "lib/lexer/Lexer.hpp"

class NewlineHandler : public Handler {
public:
  OptToken Scan(Lexer &lx) override;
};

#endif // NEWLINEHANDLER_HPP_
