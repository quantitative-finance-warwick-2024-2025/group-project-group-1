#pragma once
#include "Order.hpp"
#include "OrderBook.hpp"

class LimitOrder : public Order, public std::enable_shared_from_this<LimitOrder>
{
  private:
    double m_limitPrice;

  public:
    LimitOrder(OrderBook& orderBook, int qty, bool isBuy, double limitPrice);
    double getLimitPrice();
    void execute();
};
