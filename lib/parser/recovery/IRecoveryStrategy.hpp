#ifndef IRECOVERYSTRATEGY_HPP_
#define IRECOVERYSTRATEGY_HPP_

#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

class IRecoveryStrategy {
public:
  virtual ~IRecoveryStrategy() {
  }
  virtual void SyncToStmtEnd(ITokenStream& ts) = 0;
  virtual void SyncToBlockEnd(ITokenStream& ts) = 0;
};

#endif // IRECOVERYSTRATEGY_HPP_
