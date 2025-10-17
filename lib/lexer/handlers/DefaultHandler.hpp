#ifndef DEFAULTHANDLER_HPP_
#define DEFAULTHANDLER_HPP_

#include "Handler.hpp"

class DefaultHandler : public Handler {
public:
  OptToken Scan(SourceCodeWrapper& wrapper) override;
};

#endif // DEFAULTHANDLER_HPP_
