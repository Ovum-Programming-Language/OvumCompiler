#ifndef NEWLINEHANDLER_HPP_
#define NEWLINEHANDLER_HPP_

#include "Handler.hpp"

class NewlineHandler : public Handler {
public:
  OptToken Scan(SourceCodeWrapper& wrapper) override;
};

#endif // NEWLINEHANDLER_HPP_
