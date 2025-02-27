#include "StopOrder.hpp"
#include "OrderBook.hpp"
#include <string>
#include <iostream>

// Create the constructor of the Stop Order
StopOrder::StopOrder(OrderBook& orderBook, int qty, bool isBuy, double stopPrice)
  :
  Order(orderBook, qty, isBuy),
  m_stopPrice(stopPrice),
  m_triggered(false)
{}

// Method for executing the StopOrder
void StopOrder::execute()
{
  std::cout << "(OrderID: " << m_orderId << ") " << getSubmitTime() << " - Stop Market Order " << (m_isBuy ? "to buy" : "to sell") << " " << m_qty << " with stop price " << m_stopPrice << std::endl;
}

// Method for checking if StopOrder has been triggered 
bool StopOrder::checkTrigger()
{
  return false;
}
