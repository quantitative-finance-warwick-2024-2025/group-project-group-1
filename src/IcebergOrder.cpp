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
  std::cout << "Iceberg Order for executing total qty = " << m_qty << " in chunks of qty = " << m_hiddenQty << std::endl;
}

// Method for refreshing IcebergOrder
void IcebergOrder::refreshDisplayOrder()
{}
