#include "LimitOrder.hpp"
#include <string>
#include <iostream>

// Constructor of Limit Order
LimitOrder::LimitOrder(int orderId, int qty, bool isBuy, std::string& timeStamp, double limitPrice)
  :
  Order(orderId, qty, isBuy, timeStamp),
  m_limitPrice(limitPrice)
{}

// Method to execute limit order
void LimitOrder::execute()
{
  std::cout << "Limit order " << (m_isBuy ? "to buy" : "to sell") << " " << m_qty << " with limit price " << m_limitPrice << std::endl;
}
