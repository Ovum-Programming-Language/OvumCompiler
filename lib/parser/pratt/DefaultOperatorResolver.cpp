#include "lib/parser/pratt/DefaultOperatorResolver.hpp"

#include <algorithm>
#include <string_view>

#include "lib/parser/ast/nodes/exprs/tags/OpTags.hpp"

namespace ovum::compiler::parser {

namespace {
bool LexIs(const Token& t, std::string_view s) {
  return t.GetLexeme() == s;
}

constexpr int kBP_Assign = 10;
constexpr int kBP_Elvis = 20; // right-assoc
constexpr int kBP_Or = 30;
constexpr int kBP_And = 40;
constexpr int kBP_Xor = 50;
constexpr int kBP_Eq = 60;
constexpr int kBP_Rel = 70;
constexpr int kBP_Add = 80;
constexpr int kBP_Mul = 90;
constexpr int kBP_Post = 100;

std::vector<InfixSpec>& InfixTable() {
  static std::vector<InfixSpec> specs;
  if (!specs.empty()) {
    return specs;
  }

  auto add = [&](std::string_view lex, int bp, const IBinaryOpTag& tag) {
    InfixSpec s(bp, bp, false, const_cast<IBinaryOpTag*>(&tag), false);
    s.SetMatcher([lex](const Token& t) { return LexIs(t, lex); });
    specs.push_back(std::move(s));
  };

  add("+", kBP_Add, OpTags::Add());
  add("-", kBP_Add, OpTags::Sub());
  add("*", kBP_Mul, OpTags::Mul());
  add("/", kBP_Mul, OpTags::Div());
  add("%", kBP_Mul, OpTags::Mod());

  add("==", kBP_Eq, OpTags::Eq());
  add("!=", kBP_Eq, OpTags::Ne());

  add("<", kBP_Rel, OpTags::Lt());
  add("<=", kBP_Rel, OpTags::Le());
  add(">", kBP_Rel, OpTags::Gt());
  add(">=", kBP_Rel, OpTags::Ge());

  add("&&", kBP_And, OpTags::And());
  add("||", kBP_Or, OpTags::Or());
  add("^", kBP_Xor, OpTags::Xor());

  {
    InfixSpec elvis(kBP_Elvis, kBP_Elvis - 1, true, nullptr, true);
    elvis.SetMatcher([](const Token& t) { return LexIs(t, "?:"); });
    specs.push_back(std::move(elvis));
  }

  return specs;
}

std::vector<PostfixSpec>& PostfixTable() {
  static std::vector<PostfixSpec> specs;
  if (!specs.empty()) {
    return specs;
  }

  specs.emplace_back([](const Token& t) { return LexIs(t, "("); }, kBP_Post, false);
  specs.emplace_back([](const Token& t) { return LexIs(t, "."); }, kBP_Post, false);
  specs.emplace_back([](const Token& t) { return LexIs(t, "?."); }, kBP_Post, false);
  specs.emplace_back([](const Token& t) { return LexIs(t, "as"); }, kBP_Post, true);
  specs.emplace_back([](const Token& t) { return LexIs(t, "is"); }, kBP_Post, true);
  specs.emplace_back([](const Token& t) { return LexIs(t, "::"); }, kBP_Post, false);

  return specs;
}

const IUnaryOpTag* MatchPrefix(const Token& t) {
  if (LexIs(t, "-")) {
    return &OpTags::Neg();
  }

  if (LexIs(t, "+")) {
    return &OpTags::Plus();
  }

  if (LexIs(t, "!")) {
    return &OpTags::Not();
  }

  return nullptr;
}

} // namespace

std::optional<std::reference_wrapper<const InfixSpec>> DefaultOperatorResolver::FindInfix(const Token& t) const {
  auto& tbl = InfixTable();
  for (const auto& s : tbl) {
    if (s.TryMatch(t)) {
      return std::cref(s);
    }
  }

  return std::nullopt;
}

std::optional<std::reference_wrapper<const PostfixSpec>> DefaultOperatorResolver::FindPostfix(const Token& t) const {
  auto& tbl = PostfixTable();
  for (const auto& s : tbl) {
    if (s.TryMatch(t)) {
      return std::cref(s);
    }
  }

  return std::nullopt;
}

std::optional<std::reference_wrapper<const IUnaryOpTag>> DefaultOperatorResolver::FindPrefix(const Token& t) const {
  if (auto* tag = MatchPrefix(t)) {
    return std::cref(*tag);
  }

  return std::nullopt;
}

bool DefaultOperatorResolver::IsContinuation(const Token& t) const {
  if (FindInfix(t).has_value()) {
    return true;
  }

  if (FindPostfix(t).has_value()) {
    return true;
  }

  const auto& lx = t.GetLexeme();
  if (lx == "(" || lx == "[") {
    return true;
  }
  return false;
}

} // namespace ovum::compiler::parser
