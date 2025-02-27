#pragma once
#include "OrderBook.hpp"
#include "Order.hpp"

class StopLimitOrder : public Order
{
  private:
    double m_stopPrice;
    double m_limitPrice;
    bool m_triggered;

  public:
    StopLimitOrder(OrderBook& orderBook, int qty, bool isBuy, double stopPrice, double limitPrice);
    bool checkTrigger();
    void execute();
};
