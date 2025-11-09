#ifndef LEXER_LEXERERROR_HPP_
#define LEXER_LEXERERROR_HPP_

#include <stdexcept>

namespace ovum::compiler::lexer {

class LexerError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

} // namespace ovum::compiler::lexer

#endif // LEXER_LEXERERROR_HPP_
