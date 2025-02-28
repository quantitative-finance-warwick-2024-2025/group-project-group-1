#include "Order.hpp"
#include "OrderBook.hpp"
#include <string>
#include <iostream>
#include <ctime>

// Constructor
Order::Order(OrderBook& orderBook, int qty, bool isBuy)
  :
  m_submitTime(std::time(0)),
  m_orderBook(orderBook),
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

// Get isBuy
bool Order::isBuy()
{
  bool isBuy = m_isBuy;
  return isBuy;
}

// Get order id
int Order::getOrderId()
{
  int orderId = m_orderId;
  return orderId;
}

int Order::getQty()
{
  int qty = m_qty;
  return qty;
}

double Order::getLimitPrice()
{
  return 0;
}