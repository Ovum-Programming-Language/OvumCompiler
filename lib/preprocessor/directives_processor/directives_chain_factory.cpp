#include <memory>
#include <vector>

#include "lib/preprocessor/directives_processor/directives_chain_factory.hpp"

std::unique_ptr<DirectiveHandler> CreateDirectivesChain() {
  std::vector<std::unique_ptr<DirectiveHandler>> handlers;
  handlers.emplace_back(std::make_unique<DefineHandler>());
  handlers.emplace_back(std::make_unique<UndefHandler>());
  handlers.emplace_back(std::make_unique<IfdefHandler>());
  handlers.emplace_back(std::make_unique<IfndefHandler>());
  handlers.emplace_back(std::make_unique<ElseHandler>());
  handlers.emplace_back(std::make_unique<EndifHandler>());
  handlers.emplace_back(std::make_unique<DefaultHandler>());

  for (size_t j = handlers.size(); j-- > 1;) {
    handlers[j - 1]->SetNext(std::move(handlers[j]));
  }

  return std::move(handlers[0]);
}
