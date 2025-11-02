enable_testing()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_data 
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

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
)

target_link_libraries(
        ${PROJECT_NAME}_tests
        compiler_ui
        lexer
        preprocessor
        GTest::gtest_main
)

target_compile_definitions(${PROJECT_NAME}_tests PRIVATE 
    TEST_DATA_DIR="${CMAKE_CURRENT_BINARY_DIR}/test_data")

message(STATUS "Tests build type: ${CMAKE_BUILD_TYPE}")

target_include_directories(${PROJECT_NAME}_tests PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME}_tests)
