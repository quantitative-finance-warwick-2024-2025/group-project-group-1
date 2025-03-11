#pragma once
#include "Types.hpp"
#include <map>

class OrderBook {
  private:
    // Bookds for Limit Orders
    Bids bids_;
    Asks asks_;
    // Books for Stop Orders
    StopBids stopBids_;
    StopAsks stopAsks_;
    // Other
    Orders orders_;
    OrderId currentOrderId_{0};

  public:
    void submitOrder(Order order);
    void removeOrder(OrderId orderId, bool print = false);
    void printAsks();
    void printBids();
    void printOrders();
    void getBookSnapshot();
    void matchOrders();
    void checkStopOrders();
    OrderId nextOrderId();
    OrderPointer viewOrder(OrderId orderId);
    OrderPointer getBestBidOrder() const;
    OrderPointer getBestAskOrder() const;
    Quantity getAskQuantityAtLevel(Price price);
    Quantity getBidQuantityAtLevel(Price price);
    Price getMarketSpread() const;
    Price getMarketPrice() const;
    void clear();
    void removeStopOrder(OrderPointer stopOrderPointer, bool print = false);
};
