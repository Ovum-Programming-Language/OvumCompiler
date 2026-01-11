file(GLOB_RECURSE PARSER_SOURCES
        "*.cpp"
)

add_library(parser STATIC
        ${PARSER_SOURCES}
)

target_include_directories(parser
        PUBLIC
        ${CMAKE_SOURCE_DIR}
)

target_include_directories(parser
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(parser PUBLIC
        lexer
        preprocessor
        tokens
)

target_include_directories(parser PUBLIC ${PROJECT_SOURCE_DIR})

