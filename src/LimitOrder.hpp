#pragma once
#include "Order.hpp"

class LimitOrder : public Order
{
  private:
    double m_limitPrice;

  public:
    LimitOrder(int qty, bool isBuy, double limitPrice);
    void execute();
};
