#include "StateSyncToBlockEnd.hpp"

#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/recovery/SimpleRecovery.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

namespace ovum::compiler::parser {

std::string_view StateSyncToBlockEnd::Name() const {
  return "SyncToBlockEnd";
}

IState::StepResult StateSyncToBlockEnd::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SimpleRecovery recovery;
  recovery.SyncToBlockEnd(ts);
  return false;
}

} // namespace ovum::compiler::parser
