#ifndef CHARHANDLER_HPP_
#define CHARHANDLER_HPP_

#include "Handler.hpp"

class CharHandler : public Handler {
public:
  OptToken Scan(SourceCodeWrapper& wrapper) override;
};

#endif // CHARHANDLER_HPP_
