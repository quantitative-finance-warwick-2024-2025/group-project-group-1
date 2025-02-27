#pragma once
#include "Order.hpp"

class LimitOrder : public Order
{
  private:
    double m_limitPrice;

  public:
    LimitOrder(int orderId, int qty, bool isBuy, std::string& timeStamp, double limiPrice);
    void execute();
};
