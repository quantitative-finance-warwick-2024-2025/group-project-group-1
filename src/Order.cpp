#include "Order.hpp"
#include <string>
#include <iostream>

// Constructor
Order::Order(int orderId, int qty, bool isBuy, std::string& timeStamp) 
  : 
  m_orderId(orderId),
  m_qty(qty),
  m_isBuy(isBuy),
  m_timeStamp(timeStamp)
{}

// Methods
void Order::execute()
{
  std::cout << "Order with qty = " << m_qty << std::endl;
}
