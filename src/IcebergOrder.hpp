#pragma once
#include "Order.hpp"
#include "OrderBook.hpp"
#include <string>

class IcebergOrder : public Order
{
  private:
    int m_displayQty;
    double m_limitPrice;
    int m_remainingQty;

  public:
    IcebergOrder(OrderBook& orderBook, int qty, bool isBuy, int displayQty, double limitPrice);
    void execute() override;
    void refreshDisplayOrder();

    int getDisplayQty() const { return m_displayQty; }
    double getLimitPrice() const { return m_limitPrice; }
    int getRemainingQty() const { return m_remainingQty; }
};
