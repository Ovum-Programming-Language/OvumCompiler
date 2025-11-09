add_library(preprocessor STATIC
        Preprocessor.cpp
        token_processor_factory.cpp
        import_processor/TokenImportProcessor.cpp
        directives_processor/TokenDirectivesProcessor.cpp
        import_processor/FileGraph.cpp
        directives_processor/directives_chain_factory.cpp
        directives_processor/handlers/DefaultHandler.cpp
        directives_processor/handlers/DefineHandler.cpp
        directives_processor/handlers/ElseHandler.cpp
        directives_processor/handlers/EndifHandler.cpp
        directives_processor/handlers/UndefHandler.cpp
        directives_processor/handlers/IfndefHandler.cpp
        directives_processor/handlers/IfdefHandler.cpp
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
        tokens
)

target_include_directories(preprocessor PUBLIC ${PROJECT_SOURCE_DIR})

