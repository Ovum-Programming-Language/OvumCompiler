enable_testing()

add_executable(
        ${PROJECT_NAME}_tests
        main_test.cpp
        lexer_big_programs_tests.cpp
        test_functions.cpp
        test_suites/ProjectIntegrationTestSuite.cpp
        test_suites/LexerUnitTestSuite.cpp
        lexer_tests.cpp
        test_suites/PreprocessorUnitTestSuite.cpp
        preprocessor_tests.cpp
        parser_bytecode_tests.cpp
)

target_link_libraries(
        ${PROJECT_NAME}_tests
        compiler_ui
        lexer
        preprocessor
        parser
        GTest::gtest_main
)

target_compile_definitions(${PROJECT_NAME}_tests PRIVATE
        TEST_DATA_DIR="${CMAKE_CURRENT_BINARY_DIR}/test_data")

message(STATUS "Tests build type: ${CMAKE_BUILD_TYPE}")

target_include_directories(${PROJECT_NAME}_tests PUBLIC ${PROJECT_SOURCE_DIR})

add_custom_command(
    TARGET ${PROJECT_NAME}_tests
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Updating examples submodule..."
    COMMAND git -C ${CMAKE_SOURCE_DIR} submodule update --init --recursive tests/test_data/examples
    COMMAND git -C ${CMAKE_SOURCE_DIR}/tests/test_data/examples fetch origin main
    COMMAND git -C ${CMAKE_SOURCE_DIR}/tests/test_data/examples checkout main
    COMMAND git -C ${CMAKE_SOURCE_DIR}/tests/test_data/examples pull origin main
    COMMAND ${CMAKE_COMMAND} -E echo "Cleaning previous test_data..."
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/test_data
    COMMAND ${CMAKE_COMMAND} -E echo "Copying test_data..."
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/test_data ${CMAKE_CURRENT_BINARY_DIR}/test_data
    COMMENT "Updating git submodule and preparing test_data"
    VERBATIM
)

include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME}_tests)
