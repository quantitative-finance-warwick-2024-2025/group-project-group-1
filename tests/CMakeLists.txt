add_executable(order_book_tests ${SRC_FILES} order_book_tests.cpp)
target_include_directories(order_book_tests PUBLIC ${CMAKE_SOURCE_DIR}/extern/catch2 ${CMAKE_SOURCE_DIR}/src)
add_test(NAME my_test COMMAND my_test)
