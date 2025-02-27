#pragma once
#include "Order.hpp"
#include <string>

class IcebergOrder : public Order
{
  private:
    int m_displayQty;
    int m_hiddenQty;
    double m_limitPrice;

  public:
    IcebergOrder(int orderId, int qty, bool isBuy, std::string& timeStamp, int displayQty, int hiddenQty, double limitPrice);
    void execute();
    void refreshDisplayOrder();
};
