#include "StopOrder.hpp"
#include <string>
#include <iostream>

// Create the constructor of the Stop Order
StopOrder::StopOrder(int orderId, int qty, bool isBuy, std::string& timeStamp, double stopPrice, bool triggered)
  :
  Order(orderId, qty, isBuy, timeStamp),
  m_stopPrice(stopPrice),
  m_triggered(triggered)
{}

// Method for executing the StopOrder
void StopOrder::execute()
{
  std::cout << "Stop Order with QTY = " << m_qty << " and stop price = " << m_stopPrice << std::endl;
}

// Method for checking if StopOrder has been triggered 
bool StopOrder::checkTrigger()
{
  return false;
}
