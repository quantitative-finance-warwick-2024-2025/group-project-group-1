#include <iostream>
#include <string>
#include "IcebergOrder.hpp"

// Create constructor for Iceberg Order
IcebergOrder::IcebergOrder(int orderId, int qty, bool isBuy, std::string& timeStamp, int displayQty, int hiddenQty, double limitPrice)
  :
  Order(orderId, qty, isBuy, timeStamp),
  m_displayQty(displayQty),
  m_hiddenQty(hiddenQty),
  m_limitPrice(limitPrice)
{}

// Method for executing an Iceberg Order
void IcebergOrder::execute()
{
  std::cout << "Iceberg Limit Order " << (m_isBuy ? "to buy" : "to sell") << " " << m_qty << " with chunks of " << m_displayQty << " and limit price " << m_limitPrice << std::endl;
}

// Method for refreshing IcebergOrder
void IcebergOrder::refreshDisplayOrder()
{}
