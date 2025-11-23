#ifndef PARSER_SIMPLERECOVERY_HPP_
#define PARSER_SIMPLERECOVERY_HPP_

#include "IRecoveryStrategy.hpp"

namespace ovum::compiler::parser {

class SimpleRecovery : public IRecoveryStrategy {
public:
  ~SimpleRecovery() override = default;

  void SyncToStmtEnd(ITokenStream& ts) override;
  void SyncToBlockEnd(ITokenStream& ts) override;

  void SyncToStatementEnd(ITokenStream& ts) {
    SyncToStmtEnd(ts);
  } // alias
};

} // namespace ovum::compiler::parser

#endif // PARSER_SIMPLERECOVERY_HPP_
