#ifndef OPERATORHANDLER_HPP_
#define OPERATORHANDLER_HPP_

#include "Handler.hpp"

class OperatorHandler : public Handler {
public:
  OptToken Scan(SourceCodeWrapper& wrapper) override;
};

#endif // OPERATORHANDLER_HPP_
