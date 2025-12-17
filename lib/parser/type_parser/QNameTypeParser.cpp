#include "QNameTypeParser.hpp"

#include <algorithm>
#include <array>
#include <string>

#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/tokens/token_traits/MatchIdentifier.hpp"
#include "lib/parser/tokens/token_traits/MatchLexeme.hpp"
#include "lib/parser/tokens/token_traits/MatchType.hpp"

namespace ovum::compiler::parser {

namespace {

bool IsIdentifier(const Token& token) {
  MatchIdentifier matcher;
  return matcher.TryMatch(token);
}

bool IsTypeNameToken(const Token& token) {
  // Built-in types are lexed as keywords, user types as identifiers.
  if (IsIdentifier(token)) {
    return true;
  }

  static const std::array<std::string_view, 8> kBuiltins{
      "Int", "Float", "Bool", "Char", "String", "Void", "Object", "Null"};

  const std::string lex = token.GetLexeme();
  return std::find(kBuiltins.begin(), kBuiltins.end(), lex) != kBuiltins.end();
}

void SkipTrivia(ITokenStream& ts) {
  while (!ts.IsEof()) {
    const Token& t = ts.Peek();
    const std::string type = t.GetStringType();
    if (type == "COMMENT" || type == "NEWLINE") {
      ts.Consume();
      continue;
    }
    break;
  }
}

} // namespace

QNameTypeParser::QNameTypeParser(IAstFactory& factory) noexcept : factory_(std::shared_ptr<IAstFactory>(&factory, [](auto*) {})) {
}

std::unique_ptr<TypeReference> QNameTypeParser::ParseType(ITokenStream& ts, IDiagnosticSink& diags) {
  SkipTrivia(ts);

  if (ts.IsEof()) {
    diags.Error("E_TYPE_EOF", "expected type, reached end of input");
    return nullptr;
  }

  const Token& start = ts.Peek();
  if (!IsTypeNameToken(start)) {
    diags.Error("E_TYPE_NAME", "expected type name");
    return nullptr;
  }

  std::vector<std::string> parts;
  parts.emplace_back(start.GetLexeme());
  ts.Consume();

  SkipTrivia(ts);
  while (!ts.IsEof()) {
    const Token& sep = ts.Peek();
    const std::string lex = sep.GetLexeme();
    if (lex != "." && lex != "::") {
      break;
    }

    ts.Consume();
    SkipTrivia(ts);

    if (ts.IsEof() || !IsTypeNameToken(ts.Peek())) {
      diags.Error("E_TYPE_QUAL", "expected identifier after namespace separator");
      return nullptr;
    }

    parts.emplace_back(ts.Peek().GetLexeme());
    ts.Consume();
    SkipTrivia(ts);
  }

  auto type = std::make_unique<TypeReference>(parts);

  // Parse type arguments: List<Int, String>
  SkipTrivia(ts);
  if (!ts.IsEof() && ts.Peek().GetLexeme() == "<") {
    ts.Consume();
    SkipTrivia(ts);

    while (true) {
      SkipTrivia(ts);
      if (ts.IsEof()) {
        diags.Error("E_TYPE_ARGS_EOF", "unexpected end of input in type arguments");
        return nullptr;
      }

      // Check for closing bracket
      if (ts.Peek().GetLexeme() == ">") {
        ts.Consume();
        break;
      }

      // Parse type argument (recursive call)
      auto arg_type = ParseType(ts, diags);
      if (arg_type == nullptr) {
        return nullptr;
      }
      type->AddTypeArgument(std::move(*arg_type));

      SkipTrivia(ts);
      if (ts.IsEof()) {
        diags.Error("E_TYPE_ARGS_EOF", "unexpected end of input in type arguments");
        return nullptr;
      }

      const std::string lex = ts.Peek().GetLexeme();
      if (lex == ">") {
        ts.Consume();
        break;
      }
      if (lex != ",") {
        diags.Error("E_TYPE_ARGS_SEP", "expected ',' or '>' in type arguments");
        return nullptr;
      }
      ts.Consume();
    }
  }

  // Parse nullability: Type?
  SkipTrivia(ts);
  bool nullable = false;
  if (!ts.IsEof() && ts.Peek().GetLexeme() == "?") {
    nullable = true;
    ts.Consume();
  }

  if (nullable) {
    type->MakeNullable();
  }

  return type;
}

} // namespace ovum::compiler::parser
