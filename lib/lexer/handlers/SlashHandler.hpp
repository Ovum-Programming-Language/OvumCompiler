#ifndef SLASHHANDLER_HPP_
#define SLASHHANDLER_HPP_

#include "Handler.hpp"

class SlashHandler : public Handler {
public:
  OptToken Scan(SourceCodeWrapper& wrapper) override;
};

#endif // SLASHHANDLER_HPP_
