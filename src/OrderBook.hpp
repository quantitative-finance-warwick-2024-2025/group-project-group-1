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
    void clear();
    void removeStopOrder(OrderPointer stopOrderPointer, bool print = false);
    // Getter methods
    Quantity getBidQuantityAtLevel(Price price);
    Quantity getAskQuantityAtLevel(Price price);
    OrderPointer getBestAskOrder() const;
    OrderPointer getBestBidOrder() const;
    OrderPointer getOrder(OrderId orderId);
    Price getMarketSpread() const;
    Price getMarketPrice() const;
};
