add_library(lexer STATIC
        Lexer.cpp
        utils.cpp
        handlers/CharHandler.cpp
        handlers/DefaultHandler.cpp
        handlers/IdentifierHandler.cpp
        handlers/NewlineHandler.cpp
        handlers/NumberHandler.cpp
        handlers/OperatorHandler.cpp
        handlers/PunctHandler.cpp
        handlers/SlashHandler.cpp
        handlers/StringHandler.cpp
        handlers/WhitespaceHandler.cpp
)

target_include_directories(lexer
        PUBLIC
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(lexer PUBLIC cxx_std_20)
