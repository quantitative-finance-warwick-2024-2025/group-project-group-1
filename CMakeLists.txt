# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name
project(MyProgram)

# Set C++ standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra)

# Add more source files here if needed
set(SRC_FILES ${CMAKE_SOURCE_DIR}/src/CLIHandler.cpp ${CMAKE_SOURCE_DIR}/src/Order.cpp ${CMAKE_SOURCE_DIR}/src/OrderBook.cpp ${CMAKE_SOURCE_DIR}/src/MarketOrder.cpp ${CMAKE_SOURCE_DIR}/src/LimitOrder.cpp ${CMAKE_SOURCE_DIR}/src/StopOrder.cpp ${CMAKE_SOURCE_DIR}/src/StopLimitOrder.cpp ${CMAKE_SOURCE_DIR}/src/IcebergOrder.cpp)

add_executable(my_program ${SRC_FILES} ${CMAKE_SOURCE_DIR}/src/main.cpp)

####################### TESTING STUFF STARTS HERE ###########################################################
# Create the library for reuse in tests
add_library(orderbook_lib STATIC ${SRC_FILES})

enable_testing()
# add_subdirectory(tests)
# Add test executable
add_executable(test_iceberg_order ${CMAKE_SOURCE_DIR}/tests/TestIcebergOrder.cpp)

# Link dependencies
target_link_libraries(test_iceberg_order PRIVATE orderbook_lib)

# Include headers
target_include_directories(test_iceberg_order PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Register the test
add_test(NAME test_iceberg_order COMMAND test_iceberg_order)
    