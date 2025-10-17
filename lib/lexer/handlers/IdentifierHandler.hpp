#ifndef IDENTIFIERHANDLER_HPP_
#define IDENTIFIERHANDLER_HPP_

#include "Handler.hpp"

class IdentifierHandler : public Handler {
public:
  OptToken Scan(SourceCodeWrapper& wrapper) override;
};

#endif // IDENTIFIERHANDLER_HPP_
