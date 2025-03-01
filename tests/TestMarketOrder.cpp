#include "MarketOrder.hpp"
#include "OrderBook.hpp"
#include <sstream>
#include <cassert>
#include <iostream>

void testMarketOrderExecution() {
    OrderBook book;
    std::stringstream buffer;

    // Test buy order
    {
        std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
        MarketOrder buyOrder(book, 500, true);
        buyOrder.execute();
        std::cout.rdbuf(old);

        std::string output = buffer.str();
        assert(output.find("to buy 500") != std::string::npos);
        assert(output.find("OrderID: 1") != std::string::npos);
        buffer.str("");
    }

    // Test sell order
    {
        std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
        MarketOrder sellOrder(book, 300, false);
        sellOrder.execute();
        std::cout.rdbuf(old);

        std::string output = buffer.str();
        assert(output.find("to sell 300") != std::string::npos);
        assert(output.find("OrderID: 2") != std::string::npos);
    }
}

void testInvalidMarketOrder() {
    OrderBook book;
    
    try {
        MarketOrder invalidOrder(book, -100, true);  // Negative quantity
        assert(false);  // Should never reach here
    } catch (...) {
        assert(true);   // Expected exception
    }
}

int main() {
    testMarketOrderExecution();
    testInvalidMarketOrder();
    std::cout << "All market order tests passed!" << std::endl;
    return 0;
}