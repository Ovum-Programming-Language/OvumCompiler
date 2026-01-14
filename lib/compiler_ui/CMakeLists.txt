add_library(compiler_ui STATIC
        compiler_ui_functions.cpp
        compiler_ui_functions.hpp
)

target_link_libraries(compiler_ui PUBLIC
        lexer
        preprocessor
        parser
        argparser
)

target_include_directories(compiler_ui PUBLIC ${PROJECT_SOURCE_DIR})
