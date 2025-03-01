#include "LimitOrder.hpp"
#include "OrderBook.hpp"
#include <string>
#include <iostream>

// Constructor of Limit Order
LimitOrder::LimitOrder(OrderBook& orderBook, int qty, bool isBuy, double limitPrice)
  :
  Order(orderBook, qty, isBuy),
  m_limitPrice(limitPrice)
{}

// Method to execute limit order
void LimitOrder::execute()
{
  // Message the user that the order has been received
  std::cout << "Order Received - (OrderID: " << m_orderId << ") " << getSubmitTime() << " - Limit order " << (m_isBuy ? "to buy" : "to sell") << " " << m_qty << " with limit price " << m_limitPrice << std::endl;
  
  // Add the order to the book
  m_orderBook.addOrder(shared_from_this());
}

// Method to get limit price
double LimitOrder::getLimitPrice()
{
  return m_limitPrice;
}
