#include "StopLimitOrder.hpp"
#include "OrderBook.hpp"
#include <iostream>
#include <sstream>
#include <cassert>

void testBuyStopLimitOrder() {
    OrderBook book;
    
    // Create order with stop=100, limit=95
    StopLimitOrder order(book, 500, true, 100.0, 95.0);
    
    // Capture output
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    order.execute();
    std::cout.rdbuf(old);

    // Verify execution message
    std::string output = buffer.str();
    assert(output.find("to buy 500") != std::string::npos);
    assert(output.find("stop price 100") != std::string::npos);
    assert(output.find("limit price 95") != std::string::npos);
}

void testSellStopLimitOrder() {
    OrderBook book;
    
    // Create order with stop=50, limit=55
    StopLimitOrder order(book, 300, false, 50.0, 55.0);
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    order.execute();
    std::cout.rdbuf(old);

    std::string output = buffer.str();
    assert(output.find("to sell 300") != std::string::npos);
    assert(output.find("stop price 50") != std::string::npos);
    assert(output.find("limit price 55") != std::string::npos);
}

void testTriggerLogic() {
    OrderBook book;
    StopLimitOrder order(book, 100, true, 100.0, 95.0);
    
    // Initial state should be untriggered
    assert(order.checkTrigger() == false);
    
    // TODO: Add price update simulation logic
    // This would require extending your OrderBook implementation
    // to track market prices and update order triggers
}

void testOrderParameters() {
    OrderBook book;
    
    // Test negative prices
    try {
        StopLimitOrder invalidOrder(book, 100, true, -10, 95);
        assert(false); // Shouldn't reach here
    } catch (...) {
        assert(true);
    }
    
    // Test invalid quantity
    try {
        StopLimitOrder invalidOrder(book, 0, false, 50, 55);
        assert(false);
    } catch (...) {
        assert(true);
    }
}

int main() {
    testBuyStopLimitOrder();
    testSellStopLimitOrder();
    testTriggerLogic();
    testOrderParameters();
    
    std::cout << "All StopLimitOrder tests passed!" << std::endl;
    return 0;
}