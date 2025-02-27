#pragma once
#include "Order.hpp"

class StopLimitOrder : public Order
{
  private:
    double m_stopPrice;
    double m_limitPrice;
    bool m_triggered;

  public:
    StopLimitOrder(int orderId, int qty, bool isBuy, std::string& timeStamp, double stopPrice, double limitPrice, bool triggered);
    bool checkTrigger();
    void execute();
};
