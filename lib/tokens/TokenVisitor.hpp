#ifndef TOKENS_TOKENVISITOR_HPP_
#define TOKENS_TOKENVISITOR_HPP_

namespace ovum {

class EofToken;
class CommentToken;
class NewlineToken;
class PunctToken;
class OperatorToken;
class LiteralToken;
class KeywordToken;
class IdentToken;

class TokenVisitor { // NOLINT(cppcoreguidelines-special-member-functions)
public:
  virtual ~TokenVisitor() = default;

  virtual void Visit(const IdentToken& t) = 0;

  virtual void Visit(const KeywordToken& t) = 0;

  virtual void Visit(const LiteralToken& t) = 0;

  virtual void Visit(const OperatorToken& t) = 0;

  virtual void Visit(const PunctToken& t) = 0;

  virtual void Visit(const NewlineToken& t) = 0;

  virtual void Visit(const CommentToken& t) = 0;

  virtual void Visit(const EofToken& t) = 0;
};

} // namespace ovum

#endif // TOKENS_TOKENVISITOR_HPP_
