#include "MarketOrder.hpp"
#include <iostream>
#include <string>

// Constructor of MarketOrder
MarketOrder::MarketOrder(int orderId, int qty, bool isBuy, std::string& timeStamp)
  : 
  Order(orderId, qty, isBuy, timeStamp)
{}  

// Execute Method of MarketOrder
void MarketOrder::execute()
{
  std::cout << "Market Order to " << (m_isBuy ? "to buy" : "to sell") << " " << m_qty << " shares." << std::endl;
}
