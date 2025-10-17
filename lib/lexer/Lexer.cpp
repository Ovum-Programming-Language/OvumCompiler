#include "Lexer.hpp"

#include "handlers/CharHandler.hpp"
#include "handlers/ColonHandler.hpp"
#include "handlers/DefaultHandler.hpp"
#include "handlers/Handler.hpp"
#include "handlers/IdentifierHandler.hpp"
#include "handlers/NewlineHandler.hpp"
#include "handlers/NumberHandler.hpp"
#include "handlers/OperatorHandler.hpp"
#include "handlers/PunctHandler.hpp"
#include "handlers/SlashHandler.hpp"
#include "handlers/StringHandler.hpp"
#include "handlers/WhitespaceHandler.hpp"

#include "tokens/TokenFactory.hpp"

Lexer::Lexer(std::string_view src, bool keep_comments) : wrapper_(src, keep_comments) {
  RegisterDefaults();
}

std::vector<TokenPtr> Lexer::Tokenize() {
  std::vector<TokenPtr> tokens;

  while (!wrapper_.IsAtEnd()) {
    wrapper_.ResetTokenPosition();
    char c = wrapper_.Advance();
    Handler* current_handler = handlers_.at(static_cast<unsigned char>(c)).get();

    if (!current_handler) {
      current_handler = default_handler_.get();
    }

    OptToken maybe = current_handler->Scan(wrapper_);

    if (maybe && *maybe) {
      tokens.push_back(std::move(*maybe));
    }
  }

  tokens.push_back(TokenFactory::MakeEof(wrapper_.GetLine(), wrapper_.GetCol()));
  return tokens;
}

void Lexer::RegisterDefaults() {
  for (auto& p : handlers_) {
    p.reset();
  }

  default_handler_.reset();

  SetHandler(' ', std::make_unique<WhitespaceHandler>());
  SetHandler('\t', std::make_unique<WhitespaceHandler>());
  SetHandler('\r', std::make_unique<WhitespaceHandler>());

  SetHandler('\n', std::make_unique<NewlineHandler>());

  for (unsigned char c = 'a'; c <= 'z'; ++c) {
    SetHandler(c, std::make_unique<IdentifierHandler>());
  }

  for (unsigned char c = 'A'; c <= 'Z'; ++c) {
    SetHandler(c, std::make_unique<IdentifierHandler>());
  }

  SetHandler((unsigned char) '_', std::make_unique<IdentifierHandler>());

  for (unsigned char d = '0'; d <= '9'; ++d) {
    SetHandler(d, std::make_unique<NumberHandler>());
  }

  SetHandler((unsigned char) '.', std::make_unique<NumberHandler>());

  SetHandler((unsigned char) '"', std::make_unique<StringHandler>());
  SetHandler((unsigned char) '\'', std::make_unique<CharHandler>());

  SetHandler((unsigned char) '/', std::make_unique<SlashHandler>());
  SetHandler((unsigned char) ':', std::make_unique<ColonHandler>());

  const std::string opchars = "+-*%<>=!&|^~?.";

  for (unsigned char c : opchars) {
    SetHandler(c, std::make_unique<OperatorHandler>());
  }

  const std::string puncts = ",;(){}[]";

  for (unsigned char c : puncts) {
    SetHandler(c, std::make_unique<PunctHandler>());
  }

  SetDefaultHandler(std::make_unique<DefaultHandler>());
}
