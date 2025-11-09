#ifndef PARSER_SIMPLERECOVERY_HPP_
#define PARSER_SIMPLERECOVERY_HPP_

#include "IRecoveryStrategy.hpp"

namespace ovum::compiler::parser {

class SimpleRecovery : IRecoveryStrategy {
public:
  ~SimpleRecovery() override = default;

  void SyncToStmtEnd(ITokenStream& ts) override;
  void SyncToBlockEnd(ITokenStream& ts) override;
};

} // namespace ovum::compiler::parser

#endif // PARSER_SIMPLERECOVERY_HPP_
