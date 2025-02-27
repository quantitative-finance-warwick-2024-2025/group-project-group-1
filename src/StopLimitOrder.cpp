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
  std::cout << "Stop Limit Order " << (m_isBuy ? "to buy" : "to sell") << " " << m_qty << " with stop price " << m_stopPrice << " and limit price " << m_limitPrice << std::endl;
}

// Method for checking if the limit order has been triggered
bool StopLimitOrder::checkTrigger()
{
  return false;
}
