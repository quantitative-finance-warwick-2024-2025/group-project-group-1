#pragma once
#include "Order.hpp"
#include "OrderBook.hpp"

class MarketOrder : public Order
{
  public:
    MarketOrder(OrderBook& orderBook, int qty, bool isBuy);
    void execute();
};
