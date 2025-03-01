#include <iostream>
#include <string>
#include "IcebergOrder.hpp"
#include "OrderBook.hpp"

// Create constructor for Iceberg Order
IcebergOrder::IcebergOrder(OrderBook& orderBook, int qty, bool isBuy, int displayQty, double limitPrice)
  :
  Order(orderBook, qty, isBuy),
  m_displayQty(displayQty),
  m_limitPrice(limitPrice),
  m_remainingQty(qty)
{}

// Method for executing an Iceberg Order
void IcebergOrder::execute()
{
  std::cout << "(OrderID: " << m_orderId << ") " << getSubmitTime() 
  << " - Iceberg Limit Order " << (m_isBuy ? "to buy" : "to sell") 
  << " " << m_qty << " with chunks of " << m_displayQty 
  << " and limit price " << m_limitPrice << std::endl;
}

// Method for refreshing IcebergOrder
void IcebergOrder::refreshDisplayOrder() {
  if(m_remainingQty > 0) {
    int newDisplayQty = (m_remainingQty >= m_displayQty) ? m_displayQty : m_remainingQty;
    m_remainingQty -= newDisplayQty;

    std::cout << "(OrderID: " << m_orderId <<  ") - Refreshing Iceberg Order: "
              << newDisplayQty << " shares now visible, " 
              << m_remainingQty << "remaining." << std::endl;
  } else {
    std::cout << "(OrderID: " << m_orderId << ") - Iceberg Order Fully Executed. " << std::endl;
  }
}
