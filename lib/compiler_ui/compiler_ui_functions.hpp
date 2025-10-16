#ifndef COMPILER_UI_FUNCTIONS_HPP_
#define COMPILER_UI_FUNCTIONS_HPP_

#include <cstdint>
#include <string>
#include <vector>

int32_t StartCompilerConsoleUI(const std::vector<std::string>& args, std::ostream& out, std::ostream& err);

#endif // COMPILER_UI_FUNCTIONS_HPP_
