#include "StateSyncToStmtEnd.hpp"

#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/recovery/SimpleRecovery.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

namespace ovum::compiler::parser {

std::string_view StateSyncToStmtEnd::Name() const {
  return "SyncToStmtEnd";
}

IState::StepResult StateSyncToStmtEnd::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SimpleRecovery recovery;
  recovery.SyncToStmtEnd(ts);
  return false;
}

} // namespace ovum::compiler::parser
