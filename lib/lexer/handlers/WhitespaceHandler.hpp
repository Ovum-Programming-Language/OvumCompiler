#ifndef WHITESPACEHANDLER_HPP_
#define WHITESPACEHANDLER_HPP_

#include "Handler.hpp"

class WhitespaceHandler : public Handler {
public:
  OptToken Scan(SourceCodeWrapper& wrapper) override;
};

#endif // WHITESPACEHANDLER_HPP_
