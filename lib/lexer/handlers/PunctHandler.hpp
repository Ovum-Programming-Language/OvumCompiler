#ifndef PUNCTHANDLER_HPP_
#define PUNCTHANDLER_HPP_

#include "Handler.hpp"

class PunctHandler : public Handler {
public:
  OptToken Scan(SourceCodeWrapper& wrapper) override;
};

#endif // PUNCTHANDLER_HPP_
