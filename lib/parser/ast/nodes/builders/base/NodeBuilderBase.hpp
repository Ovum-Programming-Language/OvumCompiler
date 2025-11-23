#ifndef PARSER_NODEBUILDERBASE_HPP_
#define PARSER_NODEBUILDERBASE_HPP_

#include <memory>

#include "TokenPosition.hpp"
#include "lib/parser/tokens/SourceId.hpp"
#include "lib/parser/tokens/SourceSpan.hpp"

namespace ovum::compiler::parser {

template<class T>
class NodeBuilderBase {
public:
  NodeBuilderBase() : node_(std::make_unique<T>()) {
  }

  explicit NodeBuilderBase(std::unique_ptr<T> node) : node_(std::move(node)) {
  }

  NodeBuilderBase& WithSpan(const SourceSpan& span) {
    node_->SetSpan(span);
    return *this;
  }

  NodeBuilderBase& WithSpanParts(SourceId id, TokenPosition begin, TokenPosition end) {
    node_->SetSpanParts(std::move(id), begin, end);
    return *this;
  }

  std::unique_ptr<T> Finish() {
    return std::move(node_);
  }

  T* Raw() {
    return node_.get();
  }

protected:
  std::unique_ptr<T> node_;
};
} // namespace ovum::compiler::parser

#endif // PARSER_NODEBUILDERBASE_HPP_
