#pragma once
#include "Order.hpp"

class MarketOrder : public Order
{
  public:
    MarketOrder(int qty, bool isBuy);
    void execute();
};
