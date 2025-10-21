#ifndef STRINGHANDLER_H
#define STRINGHANDLER_H

#include "Handler.hpp"

class StringHandler : public Handler {
public:
  OptToken Scan(SourceCodeWrapper& wrapper) override;
};

#endif // STRINGHANDLER_H
