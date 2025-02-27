#include <iostream>
#include <string>
#include "IcebergOrder.hpp"
#include "OrderBook.hpp"

// Create constructor for Iceberg Order
IcebergOrder::IcebergOrder(OrderBook& orderBook, int qty, bool isBuy, int displayQty, double limitPrice)
  :
  Order(orderBook, qty, isBuy),
  m_displayQty(displayQty),
  m_limitPrice(limitPrice)
{}

// Method for executing an Iceberg Order
void IcebergOrder::execute()
{
  std::cout << "(OrderID: " << m_orderId << ") " << getSubmitTime() << " - Iceberg Limit Order " << (m_isBuy ? "to buy" : "to sell") << " " << m_qty << " with chunks of " << m_displayQty << " and limit price " << m_limitPrice << std::endl;
}

// Method for refreshing IcebergOrder
void IcebergOrder::refreshDisplayOrder()
{}
