#ifndef COLONHANDLER_HPP_
#define COLONHANDLER_HPP_
#include "Handler.hpp"

class ColonHandler : public Handler {
public:
  OptToken Scan(SourceCodeWrapper& wrapper) override;
};

#endif // COLONHANDLER_HPP_
