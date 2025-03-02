#include <iostream>
#include <string>
#include "OrderBook.hpp"
#include "StopLimitOrder.hpp"

// Create the constructor for the stop limit order
StopLimitOrder::StopLimitOrder(OrderBook& orderBook, int qty, bool isBuy, double stopPrice, double limitPrice)
  :
  Order(orderBook, qty, isBuy),
  m_stopPrice(stopPrice),
  m_limitPrice(limitPrice),
  m_triggered(false)
{}

// Method for executing the stop limit order
void StopLimitOrder::execute()
{
  std::cout << "(OrderID: " << m_orderId << ") " << getSubmitTime() 
  << " - Stop Limit Order " << (m_isBuy ? "to buy" : "to sell") 
  << " " << m_qty << " with stop price " << m_stopPrice 
  << " and limit price " << m_limitPrice << std::endl;
}

// Method for checking if the limit order has been triggered
bool StopLimitOrder::checkTrigger(double marketPrice) {
  if(!m_triggered) {
    if((m_isBuy && marketPrice >= m_stopPrice) || (!m_isBuy && marketPrice <= m_stopPrice)) {
      m_triggered = true;
    }
  }
  return false;
}

