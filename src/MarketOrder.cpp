#include "MarketOrder.hpp"
#include "OrderBook.hpp"
#include <iostream>
#include <string>

// Constructor of MarketOrder
MarketOrder::MarketOrder(OrderBook& orderBook, int qty, bool isBuy)
  : 
  Order(orderBook, qty, isBuy)
{}  

// Execute Method of MarketOrder
void MarketOrder::execute()
{
  std::cout << "(OrderID: " << m_orderId << ") " << getSubmitTime() << " - Market Order to " << (m_isBuy ? "to buy" : "to sell") << " " << m_qty << " shares." << std::endl;
}
