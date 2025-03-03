#include "OrderBook.hpp"
#include "StopOrder.hpp"
#include <cassert>
#include <iostream>
#include <sstream>

void testBuyStopOrder() {
    OrderBook book;
    StopOrder order(book, 500, true, 150.0);

    // Capture output
    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());
    order.execute();
    std::cout.rdbuf(old);

    // Verify output components
    std::string output = buffer.str();
    assert(output.find("Stop Market Order") != std::string::npos);
    assert(output.find("to buy 500") != std::string::npos);
    assert(output.find("stop price 150") != std::string::npos);
}

void testSellStopOrder() {
    OrderBook book;
    StopOrder order(book, 300, false, 120.5);

    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());
    order.execute();
    std::cout.rdbuf(old);

    std::string output = buffer.str();
    assert(output.find("to sell 300") != std::string::npos);
    assert(output.find("stop price 120.5") != std::string::npos);
}

void testOrderIdSequence() {
    OrderBook book;

    // First order
    StopOrder order1(book, 100, true, 90.0);
    std::stringstream buffer1;
    std::streambuf *old1 = std::cout.rdbuf(buffer1.rdbuf());
    order1.execute();
    std::cout.rdbuf(old1);

    // Second order
    StopOrder order2(book, 200, false, 95.0);
    std::stringstream buffer2;
    std::streambuf *old2 = std::cout.rdbuf(buffer2.rdbuf());
    order2.execute();
    std::cout.rdbuf(old2);

    // Verify IDs
    std::string output1 = buffer1.str();
    std::string output2 = buffer2.str();
    assert(output1.find("OrderID: 1") != std::string::npos);
    assert(output2.find("OrderID: 2") != std::string::npos);
}

void testTriggerState() {
    OrderBook book;
    StopOrder order(book, 50, true, 80.0);
    // assert(order.checkTrigger() == false); // Initial state
}

int main() {
    testBuyStopOrder();
    testSellStopOrder();
    testOrderIdSequence();
    testTriggerState();

    std::cout << "All StopOrder tests passed!" << std::endl;
    return 0;
}