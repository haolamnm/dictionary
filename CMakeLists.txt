cmake_minimum_required(VERSION 3.10)
project(dictionary VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable testing
enable_testing()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Separate main.cpp from the rest
set(MAIN_FILE "${CMAKE_SOURCE_DIR}/src/main.cpp")
file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
list(REMOVE_ITEM SRC_FILES ${MAIN_FILE})

# Test files
file(GLOB_RECURSE TEST_FILES "${CMAKE_SOURCE_DIR}/tests/*.cpp")

# Create a library from the app logic (excluding main.cpp)
add_library(dictionary_lib ${SRC_FILES})
target_include_directories(dictionary_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Create the main app executable (linking main.cpp + dictionary_lib)
add_executable(dictionary ${MAIN_FILE})
target_link_libraries(dictionary PRIVATE dictionary_lib)

# Add GoogleTest
add_subdirectory(external/googletest)

# Create the test executable (without main.cpp)
add_executable(run_tests ${TEST_FILES})
target_link_libraries(run_tests PRIVATE dictionary_lib gtest gtest_main)
target_include_directories(run_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Register tests with CTest
add_test(NAME AllTests COMMAND run_tests)
