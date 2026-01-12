#include "lib/parser/pratt/DefaultOperatorResolver.hpp"

#include <algorithm>
#include <string_view>

#include "lib/parser/ast/nodes/exprs/tags/optags.hpp"

namespace ovum::compiler::parser {

namespace {
bool LexIs(const Token& t, std::string_view s) {
  return t.GetLexeme() == s;
}

constexpr int kBpElvis = 20;
constexpr int kBpOr = 30;
constexpr int kBpAnd = 40;
constexpr int kBpEq = 60;
constexpr int kBpRel = 70;
constexpr int kBpAdd = 80;
constexpr int kBpBitwiseOr = 82;
constexpr int kBpBitwiseXor = 83;
constexpr int kBpBitwiseAnd = 84;
constexpr int kBpShift = 85;
constexpr int kBpMul = 90;
constexpr int kBpPost = 100;

std::vector<InfixSpec>& InfixTable() {
  static std::vector<InfixSpec> specs;
  if (!specs.empty()) {
    return specs;
  }

  auto add = [&](std::string_view lex, const int bp, const IBinaryOpTag& tag) {
    InfixSpec s(bp, bp, false, &tag, false);
    s.SetMatcher([lex](const Token& t) { return LexIs(t, lex); });
    specs.push_back(std::move(s));
  };

  add("+", kBpAdd, optags::Add());
  add("-", kBpAdd, optags::Sub());
  add("*", kBpMul, optags::Mul());
  add("/", kBpMul, optags::Div());
  add("%", kBpMul, optags::Mod());

  add("==", kBpEq, optags::Eq());
  add("!=", kBpEq, optags::Ne());

  add("<=", kBpRel, optags::Le());
  add(">=", kBpRel, optags::Ge());
  add("<", kBpRel, optags::Lt());
  add(">", kBpRel, optags::Gt());

  add("&&", kBpAnd, optags::And());
  add("||", kBpOr, optags::Or());

  add("<<", kBpShift, optags::LeftShift());
  add(">>", kBpShift, optags::RightShift());
  add("&", kBpBitwiseAnd, optags::BitwiseAnd());
  add("^", kBpBitwiseXor, optags::Xor());
  add("|", kBpBitwiseOr, optags::BitwiseOr());

  {
    InfixSpec elvis(kBpElvis, kBpElvis - 1, true, nullptr, true);
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

  specs.emplace_back([](const Token& t) { return LexIs(t, "("); }, kBpPost, false);
  specs.emplace_back([](const Token& t) { return LexIs(t, "."); }, kBpPost, false);
  specs.emplace_back([](const Token& t) { return LexIs(t, "?."); }, kBpPost, false);
  specs.emplace_back([](const Token& t) { return LexIs(t, "["); }, kBpPost, false);
  specs.emplace_back([](const Token& t) { return LexIs(t, "as"); }, kBpPost, true);
  specs.emplace_back([](const Token& t) { return LexIs(t, "is"); }, kBpPost, true);
  specs.emplace_back([](const Token& t) { return LexIs(t, "::"); }, kBpPost, false);
  specs.emplace_back([](const Token& t) { return LexIs(t, "!"); }, kBpPost, false);

  return specs;
}

const IUnaryOpTag* MatchPrefix(const Token& t) {
  if (LexIs(t, "-")) {
    return &optags::Neg();
  }

  if (LexIs(t, "+")) {
    return &optags::Plus();
  }

  if (LexIs(t, "!")) {
    return &optags::Not();
  }

  if (LexIs(t, "~")) {
    return &optags::BitwiseNot();
  }

  return nullptr;
}

} // namespace

std::optional<std::reference_wrapper<const InfixSpec>> DefaultOperatorResolver::FindInfix(const Token& t) const {
  for (const auto& tbl = InfixTable(); const auto& s : tbl) {
    if (s.TryMatch(t)) {
      return std::cref(s);
    }
  }

  return std::nullopt;
}

std::optional<std::reference_wrapper<const PostfixSpec>> DefaultOperatorResolver::FindPostfix(const Token& t) const {
  for (const auto& tbl = PostfixTable(); const auto& s : tbl) {
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

  if (const auto& lx = t.GetLexeme(); lx == "(" || lx == "[") {
    return true;
  }
  return false;
}

} // namespace ovum::compiler::parser
