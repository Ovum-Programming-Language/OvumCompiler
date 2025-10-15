#ifndef PUNCTHANDLER_HPP_
#define PUNCTHANDLER_HPP_

#include "Handler.hpp"
#include "lib/lexer/Lexer.hpp"

class PunctHandler : public Handler {
public:
  OptToken Scan(Lexer& lx) override;
};

#endif // PUNCTHANDLER_HPP_
