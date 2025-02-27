#pragma once
#include "Order.hpp"

class MarketOrder : public Order
{
  public:
    MarketOrder(int orderId, int qty, bool isBuy, std::string& timeStamp);
    void execute();
};
