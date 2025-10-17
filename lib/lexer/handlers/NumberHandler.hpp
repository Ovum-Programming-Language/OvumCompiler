#ifndef NUMBERHANDLER_HPP_
#define NUMBERHANDLER_HPP_

#include "Handler.hpp"

class NumberHandler : public Handler {
public:
  OptToken Scan(SourceCodeWrapper& wrapper) override;
};

#endif // NUMBERHANDLER_HPP_
