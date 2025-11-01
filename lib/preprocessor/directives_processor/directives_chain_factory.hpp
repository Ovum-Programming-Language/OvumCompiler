#ifndef DIRECTIVES_CHAIN_FACTORY_HPP_
#define DIRECTIVES_CHAIN_FACTORY_HPP_

#include <memory>

#include "lib/preprocessor/directives_processor/handlers/DefaultHandler.hpp"
#include "lib/preprocessor/directives_processor/handlers/DefineHandler.hpp"
#include "lib/preprocessor/directives_processor/handlers/DirectiveHandler.hpp"
#include "lib/preprocessor/directives_processor/handlers/ElseHandler.hpp"
#include "lib/preprocessor/directives_processor/handlers/EndifHandler.hpp"
#include "lib/preprocessor/directives_processor/handlers/IfdefHandler.hpp"
#include "lib/preprocessor/directives_processor/handlers/IfndefHandler.hpp"
#include "lib/preprocessor/directives_processor/handlers/UndefHandler.hpp"

std::unique_ptr<DirectiveHandler> CreateDirectivesChain();

#endif // DIRECTIVES_CHAIN_FACTORY_HPP_
