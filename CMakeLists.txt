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


# Add test executable for ICebergOrder
add_executable(test_iceberg_order ${CMAKE_SOURCE_DIR}/tests/TestIcebergOrder.cpp)
target_link_libraries(test_iceberg_order PRIVATE orderbook_lib)
target_include_directories(test_iceberg_order PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME test_iceberg_order COMMAND test_iceberg_order)
    
## test Executable for LimitOrder
add_executable(test_limit_order ${CMAKE_SOURCE_DIR}/tests/TestLimitOrder.cpp)
target_link_libraries(test_limit_order PRIVATE orderbook_lib)
target_include_directories(test_limit_order PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME test_limit_order COMMAND test_limit_order)

# executable for MarketOrder
add_executable(test_market_order ${CMAKE_SOURCE_DIR}/tests/TestMarketOrder.cpp)
target_link_libraries(test_market_order PRIVATE orderbook_lib)
target_include_directories(test_market_order PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME test_market_order COMMAND test_market_order)

# Executable for StopLimitOrder
add_executable(test_stoplimit_order ${CMAKE_SOURCE_DIR}/tests/TestLimitOrder.cpp)
target_link_libraries(test_stoplimit_order PRIVATE orderbook_lib)
target_include_directories(test_stoplimit_order PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME test_stoplimit_order COMMAND test_stoplimit_order)

#Executable for StopOrder
add_executable(test_stop_order 
    tests/TestStopOrder.cpp
)
target_link_libraries(test_stop_order PRIVATE orderbook_lib)
target_include_directories(test_stop_order PRIVATE src)
add_test(NAME test_stop_order COMMAND test_stop_order)