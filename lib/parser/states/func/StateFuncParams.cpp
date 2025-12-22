#include "StateFuncParams.hpp"

#include <memory>

#include "lib/parser/ast/nodes/class_members/CallDecl.hpp"
#include "lib/parser/ast/nodes/class_members/MethodDecl.hpp"
#include "lib/parser/ast/nodes/decls/FunctionDecl.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
#include "lib/parser/tokens/token_traits/MatchIdentifier.hpp"
#include "lib/parser/tokens/token_traits/MatchLexeme.hpp"
#include "lib/parser/types/Param.hpp"
#include "type_parser/ITypeParser.hpp"

namespace ovum::compiler::parser {

namespace {

void SkipTrivia(ITokenStream& ts, bool skip_newlines = true) {
  while (!ts.IsEof()) {
    const Token& t = ts.Peek();
    const std::string type = t.GetStringType();
    if (type == "COMMENT") {
      ts.Consume();
      continue;
    }
    if (skip_newlines && type == "NEWLINE") {
      ts.Consume();
      continue;
    }
    break;
  }
}

bool IsIdentifier(const Token& token) {
  MatchIdentifier matcher;
  return matcher.TryMatch(token);
}

std::string ReadIdentifier(ContextParser& ctx, ITokenStream& ts, std::string_view code, std::string_view message) {
  SkipTrivia(ts);
  if (ts.IsEof() || !IsIdentifier(ts.Peek())) {
    if (ctx.Diags() != nullptr) {
      const Token* tok = ts.TryPeek();
      if (tok != nullptr) {
        SourceSpan span = StateBase::SpanFrom(*tok);
        ctx.Diags()->Error(code, message, span);
      } else {
        ctx.Diags()->Error(code, message);
      }
    }
    return "";
  }
  std::string name = ts.Consume()->GetLexeme();
  return name;
}

std::unique_ptr<TypeReference> ParseType(ContextParser& ctx, ITokenStream& ts) {
  if (ctx.TypeParser() == nullptr) {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_TYPE_PARSER", "type parser not available");
    }
    return nullptr;
  }
  return ctx.TypeParser()->ParseType(ts, *ctx.Diags());
}

} // namespace

std::string_view StateFuncParams::Name() const {
  return "FuncParams";
}

IState::StepResult StateFuncParams::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  // Check for FunctionDecl, MethodDecl, or CallDecl
  FunctionDecl* func = ctx.TopNodeAs<FunctionDecl>();
  MethodDecl* method = ctx.TopNodeAs<MethodDecl>();
  CallDecl* call = ctx.TopNodeAs<CallDecl>();

  if (func == nullptr && method == nullptr && call == nullptr) {
    return std::unexpected(
        StateError(std::string_view("expected FunctionDecl, MethodDecl, or CallDecl node on stack")));
  }

  if (ts.IsEof()) {
    return std::unexpected(StateError(std::string_view("unexpected end of file in function parameters")));
  }

  const Token& tok = ts.Peek();
  if (tok.GetLexeme() == ")") {
    ts.Consume();
    SkipTrivia(ts);

    // Check for return type
    if (!ts.IsEof() && ts.Peek().GetLexeme() == ":") {
      ts.Consume();
      SkipTrivia(ts);
      auto return_type = ParseType(ctx, ts);
      if (return_type != nullptr) {
        if (func != nullptr) {
          func->SetReturnType(std::move(return_type));
        } else if (method != nullptr) {
          method->SetReturnType(std::move(return_type));
        } else if (call != nullptr) {
          call->SetReturnType(std::move(return_type));
        }
      }
    }

    SkipTrivia(ts);
    ctx.PushState(StateRegistry::FuncBody());
    return true;
  }

  // Parse parameter list
  std::vector<Param> params;
  while (true) {
    SkipTrivia(ts);
    bool is_var = false;
    if (!ts.IsEof() && ts.Peek().GetLexeme() == "var") {
      ts.Consume();
      is_var = true;
      SkipTrivia(ts);
    }

    std::string name = ReadIdentifier(ctx, ts, "P_PARAM_NAME", "expected parameter name");
    if (name.empty()) {
      break;
    }

    SkipTrivia(ts);
    if (ts.IsEof() || ts.Peek().GetLexeme() != ":") {
      if (ctx.Diags() != nullptr) {
        ctx.Diags()->Error("P_PARAM_COLON", "expected ':' after parameter name");
      }
      break;
    }
    ts.Consume();

    SkipTrivia(ts);
    auto type = ParseType(ctx, ts);
    if (type == nullptr) {
      break;
    }

    params.emplace_back(name, std::move(*type));

    SkipTrivia(ts);
    if (ts.IsEof() || ts.Peek().GetLexeme() != ",") {
      break;
    }
    ts.Consume();
  }

  for (auto& param : params) {
    if (func != nullptr) {
      func->AddParam(std::move(param));
    } else if (method != nullptr) {
      method->AddParam(std::move(param));
    } else if (call != nullptr) {
      call->AddParam(std::move(param));
    }
  }

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != ")") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_FUN_PARAMS_CLOSE", std::string_view("expected ')' after parameters"));
    }
    return std::unexpected(StateError(std::string_view("expected ')' after parameters")));
  }

  ts.Consume();
  SkipTrivia(ts);

  // Check for return type
  if (!ts.IsEof() && ts.Peek().GetLexeme() == ":") {
    ts.Consume();
    SkipTrivia(ts);
    auto return_type = ParseType(ctx, ts);
    if (return_type != nullptr) {
      if (func != nullptr) {
        func->SetReturnType(std::move(return_type));
      } else if (method != nullptr) {
        method->SetReturnType(std::move(return_type));
      } else if (call != nullptr) {
        call->SetReturnType(std::move(return_type));
      }
    }
  }

  SkipTrivia(ts);
  ctx.PushState(StateRegistry::FuncBody());
  return true;
}

} // namespace ovum::compiler::parser
