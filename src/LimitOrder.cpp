#include "LimitOrder.hpp"
#include <string>
#include <iostream>

// Constructor of Limit Order
LimitOrder::LimitOrder(int qty, bool isBuy, double limitPrice)
  :
  Order(qty, isBuy),
  m_limitPrice(limitPrice)
{}

// Method to execute limit order
void LimitOrder::execute()
{
  std::cout << "(OrderID: " << m_orderId << ") " << getSubmitTime() << " - Limit order " << (m_isBuy ? "to buy" : "to sell") << " " << m_qty << " with limit price " << m_limitPrice << std::endl;
}
