#pragma once
#include "Types.hpp"
#include <map>

class OrderBook {
  private:
    Bids bids_;
    Asks asks_;
    Orders orders_;
    OrderId currentOrderId_{0};

  public:
    void submitOrder(Order order);
    OrderPointer viewOrder(OrderId orderId);
    void removeOrder(OrderId orderId);
    void printAsks();
    void printBids();
    void printOrders();
    void getBookSnapshot();
    void matchOrders();
    OrderId nextOrderId();
    OrderPointer getBestBidOrder();
    OrderPointer getBestAskOrder();
    Price getMarketSpread();
    Quantity getAskQuantityAtLevel(Price price);
    Quantity getBidQuantityAtLevel(Price price);
    Price getMarketPrice();
    void clear();
};
