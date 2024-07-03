# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.14)

# Set the project name and version
project(Breakthrough VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(${PROJECT_NAME}_LIBRARIES board.h board.cpp)

# Add the executable for the main project
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_LIBRARIES)

# Testing configuration
enable_testing()

# Include FetchContent for downloading Catch2
include(FetchContent)

# Fetch Catch2 library
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.6.0
)
FetchContent_MakeAvailable(Catch2)

# Add the test executable
add_executable(tests test.cpp)

# Link the test executable with Catch2
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain ${PROJECT_NAME}_LIBRARIES)

# Add a custom target for running tests
add_custom_target(run_tests
  COMMAND ${CMAKE_BINARY_DIR}/tests
  DEPENDS tests
)

# Register the test
include(CTest)
include(Catch)
catch_discover_tests(tests)
