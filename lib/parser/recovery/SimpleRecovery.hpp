#ifndef SIMPLERECOVERY_HPP_
#define SIMPLERECOVERY_HPP_

#include "IRecoveryStrategy.hpp"

class SimpleRecovery : IRecoveryStrategy {
public:
  ~SimpleRecovery() override = default;

  void SyncToStmtEnd(ITokenStream& ts) override;
  void SyncToBlockEnd(ITokenStream& ts) override;
};

#endif // SIMPLERECOVERY_HPP_
