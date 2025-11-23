#include "lib/parser/recovery/SimpleRecovery.hpp"

#include <string_view>
#include <tokens/Token.hpp>

namespace ovum::compiler::parser {
namespace {
bool IsLex(const Token& tok, std::string_view lex) {
  return tok.GetLexeme() == lex;
}
} // namespace

void SimpleRecovery::SyncToStmtEnd(ITokenStream& ts) {
  while (!ts.IsEof()) {
    const auto* look = ts.TryPeek();
    if (look == nullptr) {
      break;
    }
    if (IsLex(*look, "\n")) {
      ts.Consume();
      break;
    }
    if (IsLex(*look, ";")) {
      ts.Consume();
      break;
    }
    if (IsLex(*look, "}")) {
      break;
    }
    ts.Consume();
  }
}

void SimpleRecovery::SyncToBlockEnd(ITokenStream& ts) {
  int depth = 0;
  while (!ts.IsEof()) {
    const auto* look = ts.TryPeek();
    if (look == nullptr) {
      break;
    }
    if (IsLex(*look, "{")) {
      ++depth;
      ts.Consume();
      continue;
    }
    if (IsLex(*look, "}")) {
      ts.Consume();
      if (depth == 0) {
        break;
      }
      --depth;
      continue;
    }
    ts.Consume();
  }
}

} // namespace ovum::compiler::parser
