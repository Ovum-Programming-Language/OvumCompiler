add_library(preprocessor STATIC
        Preprocessor.cpp
        token_processor_factory.cpp
        TokenImportProcessor.cpp
        TokenDirectivesProcessor.cpp
)

target_include_directories(preprocessor
        PUBLIC
        ${CMAKE_SOURCE_DIR}
)

target_include_directories(preprocessor
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(preprocessor PUBLIC
        lexer
)

target_include_directories(preprocessor PUBLIC ${PROJECT_SOURCE_DIR})

