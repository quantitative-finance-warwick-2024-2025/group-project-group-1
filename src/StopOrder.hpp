#pragma once
#include "Order.hpp"
#include "OrderBook.hpp"
#include <string>

class StopOrder : public Order
{
  private:
    double m_stopPrice;
    bool m_triggered;

  public:
    StopOrder(OrderBook& orderBook, int qty, bool isBuy, double stopPrice);
    bool checkTrigger(double marketPrice);
    void execute() override;
    double getStopPrice() const { return m_stopPrice; }
};


