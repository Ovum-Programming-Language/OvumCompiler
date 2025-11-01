#ifndef DIRECTIVES_CHAIN_FACTORY_HPP_
#define DIRECTIVES_CHAIN_FACTORY_HPP_

#include <memory>

#include "lib/preprocessor/directives_processor/handlers/DirectiveHandler.hpp"

std::unique_ptr<DirectiveHandler> CreateDirectivesChain();

#endif // DIRECTIVES_CHAIN_FACTORY_HPP_
