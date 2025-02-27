#pragma once
#include "Order.hpp"
#include <string>

class IcebergOrder : public Order
{
  private:
    int m_displayQty;
    double m_limitPrice;

  public:
    IcebergOrder(int qty, bool isBuy, int displayQty, double limitPrice);
    void execute();
    void refreshDisplayOrder();
};
