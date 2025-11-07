add_library(lexer STATIC
        Lexer.cpp
        SourceCodeWrapper.cpp
        handlers/CharHandler.cpp
        handlers/ColonHandler.cpp
        handlers/DefaultHandler.cpp
        handlers/DotCompositeHandler.cpp
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
)

target_include_directories(lexer
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(lexer PUBLIC
        tokens
)
