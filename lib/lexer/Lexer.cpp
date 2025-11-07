#include "Lexer.hpp"

#include <string>
#include <utility>

#include "handlers/CharHandler.hpp"
#include "handlers/ColonHandler.hpp"
#include "handlers/DefaultHandler.hpp"
#include "handlers/DotCompositeHandler.hpp"
#include "handlers/IdentifierHandler.hpp"
#include "handlers/NewlineHandler.hpp"
#include "handlers/NumberHandler.hpp"
#include "handlers/OperatorHandler.hpp"
#include "handlers/PunctHandler.hpp"
#include "handlers/SlashHandler.hpp"
#include "handlers/StringHandler.hpp"
#include "handlers/WhitespaceHandler.hpp"

#include "lib/tokens/TokenFactory.hpp"

namespace ovum::compiler::lexer {

namespace {
constexpr const char* kOperatorChars = "+-*%<>=!&|^~?";
constexpr const char* kPunctChars = ",;(){}[]";
} // namespace

Lexer::Lexer(std::string_view src, bool keep_comments) :
    wrapper_(src, keep_comments), handlers_(MakeDefaultHandlers()), default_handler_(MakeDefaultHandler()) {
}

std::expected<std::vector<TokenPtr>, LexerError> Lexer::Tokenize() {
  std::vector<TokenPtr> tokens;
  tokens.reserve(kDefaultTokenReserve);

  while (!wrapper_.IsAtEnd()) {
    wrapper_.ResetTokenPosition();

    char ch_read = wrapper_.Advance();
    Handler* current_handler = handlers_.at(static_cast<unsigned char>(ch_read)).get();

    if (!current_handler) {
      current_handler = default_handler_.get();
    }

    auto maybe_token_result = current_handler->Scan(wrapper_);

    if (!maybe_token_result) {
      return std::unexpected(maybe_token_result.error());
    }

    OptToken maybe_token = maybe_token_result.value();

    if (maybe_token && *maybe_token) {
      tokens.push_back(std::move(*maybe_token));
    }
  }

  tokens.push_back(TokenFactory::MakeEof(wrapper_.GetLine(), wrapper_.GetCol()));
  return tokens;
}

void Lexer::SetHandler(unsigned char ch, std::unique_ptr<Handler> handler) {
  handlers_.at(ch) = std::move(handler);
}

void Lexer::SetDefaultHandler(std::unique_ptr<Handler> handler) {
  default_handler_ = std::move(handler);
}

std::array<std::unique_ptr<Handler>, kDefaultTokenReserve> Lexer::MakeDefaultHandlers() {
  std::array<std::unique_ptr<Handler>, kDefaultTokenReserve> table{};
  table.at(static_cast<unsigned char>(' ')) = std::make_unique<WhitespaceHandler>();
  table.at(static_cast<unsigned char>('\t')) = std::make_unique<WhitespaceHandler>();
  table.at(static_cast<unsigned char>('\r')) = std::make_unique<WhitespaceHandler>();

  table.at(static_cast<unsigned char>('\n')) = std::make_unique<NewlineHandler>();

  for (unsigned char ch = 'a'; ch <= 'z'; ++ch) {
    table.at(ch) = std::make_unique<IdentifierHandler>();
  }

  for (unsigned char ch = 'A'; ch <= 'Z'; ++ch) {
    table.at(ch) = std::make_unique<IdentifierHandler>();
  }

  table.at(static_cast<unsigned char>('_')) = std::make_unique<IdentifierHandler>();
  table.at(static_cast<unsigned char>('#')) = std::make_unique<IdentifierHandler>();

  for (unsigned char digit = '0'; digit <= '9'; ++digit) {
    table.at(digit) = std::make_unique<NumberHandler>();
  }

  table.at(static_cast<unsigned char>('.')) = std::make_unique<DotCompositeHandler>();

  table.at(static_cast<unsigned char>('"')) = std::make_unique<StringHandler>();
  table.at(static_cast<unsigned char>('\'')) = std::make_unique<CharHandler>();
  table.at(static_cast<unsigned char>('/')) = std::make_unique<SlashHandler>();

  table.at(static_cast<unsigned char>(':')) = std::make_unique<ColonHandler>();

  for (const unsigned char ch : std::string(kOperatorChars)) {
    table.at(ch) = std::make_unique<OperatorHandler>();
  }

  for (const unsigned char ch : std::string(kPunctChars)) {
    table.at(ch) = std::make_unique<PunctHandler>();
  }

  return table;
}

std::unique_ptr<Handler> Lexer::MakeDefaultHandler() {
  return std::make_unique<DefaultHandler>();
}

} // namespace ovum::compiler::lexer
