#include "lib/parser/ast/AstVisitor.hpp"
#include "lib/parser/ast/nodes/exprs/Call.hpp"

#include <utility>

void Call::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const Expr& Call::Callee() const noexcept {
  return *callee_;
}

Expr& Call::MutableCallee() noexcept {
  return *callee_;
}

void Call::SetCallee(std::unique_ptr<Expr> new_callee) {
  callee_ = std::move(new_callee);
}

std::unique_ptr<Expr> Call::ReplaceCallee(std::unique_ptr<Expr> new_callee) {
  auto old_callee = std::move(callee_);
  callee_ = std::move(new_callee);
  return old_callee;
}

const std::vector<std::unique_ptr<Expr>>& Call::Args() const noexcept {
  return args_;
}

std::vector<std::unique_ptr<Expr>>& Call::MutableArgs() noexcept {
  return args_;
}

void Call::AddArg(std::unique_ptr<Expr> new_arg) {
  args_.emplace_back(std::move(new_arg));
}

void Call::ClearArgs() {
  args_.clear();
}
