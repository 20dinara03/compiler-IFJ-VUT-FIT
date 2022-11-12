cmake_minimum_required(VERSION 3.14)
project(google_test)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE tests_SRC
        "tests/*.cpp"
        "tests/*.cc"
)

add_executable(
        unit_tests
        ${tests_SRC}
)

target_link_libraries(
        unit_tests
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)


file(GLOB_RECURSE code_SRC
        "src/**/*.h"
        "src/**/*.c"
)

add_executable(
        IFJ2022
        ${code_SRC}
)
