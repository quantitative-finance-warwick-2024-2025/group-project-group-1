#include "IcebergOrder.hpp"
#include "OrderBook.hpp"
#include <iostream>

int main() {    
    OrderBook orderBook;
    IcebergOrder order(orderBook, 1000,true, 100, 12.50);

    // Output the result of execute() to stdout
    order.execute(); // Output captured to a file later
    return 0;
}