# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name
project(CPP_LOB)

# Set C++ standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra)

# Add more source files here if needed
set(SRC_FILES ${CMAKE_SOURCE_DIR}/src/Order.cpp ${CMAKE_SOURCE_DIR}/src/OrderBook.cpp ${CMAKE_SOURCE_DIR}/src/CLIHandler.cpp)

add_executable(cpp_lob_cli ${SRC_FILES} ${CMAKE_SOURCE_DIR}/src/main.cpp)

####################### TESTING STUFF STARTS HERE ###########################################################

enable_testing()
add_subdirectory(tests)