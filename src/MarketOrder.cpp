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
  std::cout << "Order Received - (OrderID: " << m_orderId << ") " << getSubmitTime() << " - Market Order to " << (m_isBuy ? "to buy" : "to sell") << " " << m_qty << " shares." << std::endl;

  // Check if there is liquidity in the market
  if ((m_isBuy && m_orderBook.getAskSize() == 0) || (!m_isBuy && m_orderBook.getBidSize() == 0))
  {
    std::cout << "Order Failed - Market has no liquiditiy" << std::endl;
  }

}
