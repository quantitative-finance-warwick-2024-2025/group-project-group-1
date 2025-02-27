#include <iostream>
#include <string>
#include "StopLimitOrder.hpp"

// Create the constructor for the stop limit order
StopLimitOrder::StopLimitOrder(int orderId, int qty, bool isBuy, std::string& timeStamp, double stopPrice, double limitPrice, bool triggered)
  :
  Order(orderId, qty, isBuy, timeStamp),
  m_stopPrice(stopPrice),
  m_limitPrice(limitPrice),
  m_triggered(triggered)
{}

// Method for executing the stop limit order
void StopLimitOrder::execute()
{
  std::cout << "Stop Limit Order with Limit Price = " << m_limitPrice << " and qty = " << m_qty << std::endl;
}

// Method for checking if the limit order has been triggered
bool StopLimitOrder::checkTrigger()
{
  return false;
}
