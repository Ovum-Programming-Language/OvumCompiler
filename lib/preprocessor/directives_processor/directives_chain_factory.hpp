#ifndef PREPROCESSOR_DIRECTIVES_CHAIN_FACTORY_HPP_
#define PREPROCESSOR_DIRECTIVES_CHAIN_FACTORY_HPP_

#include <memory>

#include "lib/preprocessor/directives_processor/handlers/DirectiveHandler.hpp"

namespace ovum::compiler::preprocessor {

std::unique_ptr<DirectiveHandler> CreateDirectivesChain();

} // namespace ovum::compiler::preprocessor

#endif // PREPROCESSOR_DIRECTIVES_CHAIN_FACTORY_HPP_
