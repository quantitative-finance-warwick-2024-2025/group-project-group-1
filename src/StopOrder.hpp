#pragma once
#include "Order.hpp"
#include <string>

class StopOrder : public Order
{
  private:
    double m_stopPrice;
    bool m_triggered;

  public:
    StopOrder(int orderId, int qty, bool isBuy, std::string& timeStamp, double stopPrice, bool triggered);
    bool checkTrigger();
    void execute();
};

