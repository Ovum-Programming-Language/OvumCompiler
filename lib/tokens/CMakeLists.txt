add_library(tokens STATIC
        CommentToken.cpp
        EofToken.cpp
        IdentToken.cpp
        KeywordToken.cpp
        LiteralToken.cpp
        NewlineToken.cpp
        OperatorToken.cpp
        PunctToken.cpp
        TokenFactory.cpp
        TokenPosition.cpp
        values/BoolValue.cpp
        values/CharValue.cpp
        values/FloatValue.cpp
        values/IntValue.cpp
        values/StringValue.cpp
)

target_include_directories(tokens
        PUBLIC
        ${CMAKE_SOURCE_DIR}
)

target_include_directories(tokens
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

