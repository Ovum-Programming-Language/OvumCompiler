cmake_minimum_required(VERSION 3.12)

project(
        ovumc
        VERSION 0.1
        DESCRIPTION "Compiler for Ovum programming language written in C++"
        LANGUAGES CXX
)

include(cmake/SetCompilerOptions.cmake)
include(cmake/IncludeExternalLibraries.cmake)

add_subdirectory(lib)
add_subdirectory(bin)


enable_testing()
add_subdirectory(tests)
