include(FetchContent)

# GoogleTest
# For Windows: prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

FetchContent_MakeAvailable(googletest)


# ArgParser
FetchContent_Declare(
    argparser
    GIT_REPOSITORY https://github.com/bialger/ArgParser.git
    GIT_TAG v1.3.5
)

FetchContent_MakeAvailable(argparser)


# Ovum Common
FetchContent_Declare(
    ovumcommon
    GIT_REPOSITORY https://github.com/Ovum-Programming-Language/OvumCommon.git
    GIT_TAG main
)

FetchContent_MakeAvailable(ovumcommon)
