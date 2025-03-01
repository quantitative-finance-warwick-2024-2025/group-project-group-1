#include "LimitOrder.hpp"
#include "OrderBook.hpp"
#include <iostream>
#include <sstream>
#include <cassert>

void testLimitOrderExecution() {
    OrderBook book;
    std::stringstream buffer;
    
    // Test buy order
    {
        std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
        LimitOrder buyOrder(book, 500, true, 150.0);
        buyOrder.execute();
        std::cout.rdbuf(old);
        
        std::string output = buffer.str();
        assert(output.find("to buy 500") != std::string::npos);
        assert(output.find("150.0") != std::string::npos);
        buffer.str(""); // Clear buffer
    }

    // Test sell order
    {
        std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
        LimitOrder sellOrder(book, 300, false, 155.5);
        sellOrder.execute();
        std::cout.rdbuf(old);
        
        std::string output = buffer.str();
        assert(output.find("to sell 300") != std::string::npos);
        assert(output.find("155.5") != std::string::npos);
    }
}

int main() {
    testLimitOrderExecution();
    std::cout << "Limit Order test passed!" << std::endl;
    return 0;
}