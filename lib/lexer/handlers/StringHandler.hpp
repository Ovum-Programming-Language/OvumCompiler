#ifndef STRINGHANDLER_H
#define STRINGHANDLER_H

#include "Handler.hpp"
#include "lib/lexer/Lexer.hpp"

class StringHandler : public Handler {
public:
  OptToken Scan(Lexer& lx) override;
};

#endif // STRINGHANDLER_H
