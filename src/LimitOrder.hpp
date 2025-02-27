#pragma once
#include "Order.hpp"
#include "OrderBook.hpp"

class LimitOrder : public Order
{
  private:
    double m_limitPrice;

  public:
    LimitOrder(OrderBook& orderBook, int qty, bool isBuy, double limitPrice);
    void execute();
};
