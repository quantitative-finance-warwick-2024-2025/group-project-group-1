#include "CLIHandler.hpp"
#include "Order.hpp"
#include "OrderBook.hpp"
#include "Types.hpp"
#include <iostream>

int main() {
    OrderBook orderBook;
    CLIHandler cliHandler(orderBook);
    cliHandler.run();
    // Market Order
    // Order marketOrder1(Side::BUY, orderBook.nextOrderId(), 50);
    // Order marketOrder2(Side::SELL, orderBook.nextOrderId(), 50);
    // Order marketOrder3(Side::BUY, orderBook.nextOrderId(), 40);
    // Order marketOrder4(Side::SELL, orderBook.nextOrderId(), 40);

    // Order limitOrder1(Side::BUY, orderBook.nextOrderId(), 100.10, 60);
    // Order limitOrder2(Side::SELL, orderBook.nextOrderId(), 100.25, 70);
    // Order limitOrder3(Side::BUY, orderBook.nextOrderId(), 100.55, 60);
    // Order limitOrder4(Side::SELL, orderBook.nextOrderId(), 100.35, 170);
    // Order limitOrder5(Side::BUY, orderBook.nextOrderId(), 100.75, 60);
    // Order limitOrder6(Side::SELL, orderBook.nextOrderId(), 100.82, 70);
    // Order limitOrder7(Side::BUY, orderBook.nextOrderId(), 100.15, 60);
    // Order limitOrder8(Side::SELL, orderBook.nextOrderId(), 100.05, 170);

    // orderBook.addOrder(limitOrder1);
    // orderBook.getBookSnapshot();
    // orderBook.addOrder(limitOrder2);
    // orderBook.getBookSnapshot();
    // orderBook.addOrder(limitOrder3);
    // orderBook.getBookSnapshot();
    // orderBook.addOrder(limitOrder4);
    // orderBook.getBookSnapshot();
    // orderBook.addOrder(limitOrder5);
    // orderBook.getBookSnapshot();
    // orderBook.addOrder(limitOrder6);
    // orderBook.getBookSnapshot();
    // orderBook.addOrder(limitOrder7);
    // orderBook.getBookSnapshot();
    // orderBook.addOrder(limitOrder8);
    // orderBook.getBookSnapshot();
    // orderBook.executeOrder(marketOrder1);
    // orderBook.getBookSnapshot();
    // orderBook.executeOrder(marketOrder2);
    // orderBook.getBookSnapshot();
    // orderBook.executeOrder(marketOrder3);
    // orderBook.getBookSnapshot();
    // orderBook.executeOrder(marketOrder4);
    // orderBook.getBookSnapshot();

    return 0;
}
