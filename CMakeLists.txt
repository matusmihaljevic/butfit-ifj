cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Enable testing
enable_testing()

# Add Google Test library
add_subdirectory(/usr/src/gtest gtest)

# Link Google Test libraries
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Add your source and test files
add_executable(runTests lexer_test.cpp lexer_test.h scanner.c dstring.c error.c)

# Link test executable against Google Test libraries
target_link_libraries(runTests gtest gtest_main)

# Add a test
add_test(NAME lexer_test COMMAND runTests)


