#include "Order.hpp"
#include "OrderBook.hpp"
#include <string>
#include <iostream>
#include <ctime>

// Constructor
Order::Order(OrderBook& orderBook, int qty, bool isBuy)
  :
  m_orderBook(orderBook),
  m_submitTime(std::time(0)),
  m_orderId(orderBook.generateOrderId()),
  m_qty(qty),
  m_isBuy(isBuy)
{}

// Methods
void Order::execute()
{
  std::cout << "Order with qty = " << m_qty << std::endl;
}

// Get Submit Time
std::string Order::getSubmitTime()
{
  // Convert time_t to string
  std::string submitTime = std::ctime(&m_submitTime);

  // Remove the \n character from the end if it exists
  if (!submitTime.empty() && submitTime.back() == '\n')
  {
    submitTime.pop_back();
  }

  return submitTime;
}
