#ifndef PARSER_IRECOVERYSTRATEGY_HPP_
#define PARSER_IRECOVERYSTRATEGY_HPP_

#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

namespace ovum::compiler::parser {

class IRecoveryStrategy { // NOLINT(cppcoreguidelines-special-member-functions)
public:
  virtual ~IRecoveryStrategy() = default;
  virtual void SyncToStmtEnd(ITokenStream& ts) = 0;
  virtual void SyncToBlockEnd(ITokenStream& ts) = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_IRECOVERYSTRATEGY_HPP_
