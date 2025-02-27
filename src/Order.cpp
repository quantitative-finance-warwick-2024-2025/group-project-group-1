#include "Order.hpp"
#include <string>
#include <iostream>
#include <ctime>

// Constructor
Order::Order(int qty, bool isBuy)
{
  // Get the timestamp
  m_submitTime = std::time(0);
  // Get the Order ID
  m_orderId = 101;

  m_qty = qty;
  m_isBuy = isBuy;
}

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
