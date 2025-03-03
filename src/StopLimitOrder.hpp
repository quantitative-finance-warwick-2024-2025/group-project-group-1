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
    bool checkTrigger(double marketPrice);
    void execute() override;
    // getters
    double getStopPrice() const { return m_stopPrice; }
    double getLimitPrice() const { return m_limitPrice; }
    bool isTriggered() const { return m_triggered; }
};
