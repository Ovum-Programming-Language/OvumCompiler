#include "directives_chain_factory.hpp"

#include <memory>

#include "lib/preprocessor/directives_processor/handlers/DefaultHandler.hpp"
#include "lib/preprocessor/directives_processor/handlers/DefineHandler.hpp"
#include "lib/preprocessor/directives_processor/handlers/ElseHandler.hpp"
#include "lib/preprocessor/directives_processor/handlers/EndifHandler.hpp"
#include "lib/preprocessor/directives_processor/handlers/IfdefHandler.hpp"
#include "lib/preprocessor/directives_processor/handlers/IfndefHandler.hpp"
#include "lib/preprocessor/directives_processor/handlers/UndefHandler.hpp"

namespace ovum::compiler::preprocessor {

std::unique_ptr<DirectiveHandler> CreateDirectivesChain() {
  auto define_handler = std::make_unique<DefineHandler>();
  auto undef_handler = std::make_unique<UndefHandler>();
  auto ifdef_handler = std::make_unique<IfdefHandler>();
  auto ifndef_handler = std::make_unique<IfndefHandler>();
  auto else_handler = std::make_unique<ElseHandler>();
  auto endif_handler = std::make_unique<EndifHandler>();
  auto default_handler = std::make_unique<ovum::compiler::preprocessor::DefaultHandler>();

  endif_handler->SetNext(std::move(default_handler));
  else_handler->SetNext(std::move(endif_handler));
  ifndef_handler->SetNext(std::move(else_handler));
  ifdef_handler->SetNext(std::move(ifndef_handler));
  undef_handler->SetNext(std::move(ifdef_handler));
  define_handler->SetNext(std::move(undef_handler));

  return std::move(define_handler);
}

} // namespace ovum::compiler::preprocessor
