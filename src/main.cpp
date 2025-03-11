#include "CLIHandler.hpp"
#include "OrderBook.hpp"
#include <iostream>

int main() {
    OrderBook orderBook;
    CLIHandler cliHandler(orderBook);
    cliHandler.run();
    return 0;
}
