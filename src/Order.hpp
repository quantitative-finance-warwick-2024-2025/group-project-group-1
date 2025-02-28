#pragma once
#include "OrderBook.hpp"
#include <string>
#include <ctime>

class Order
{
  protected:
    OrderBook& m_orderBook;
    int m_orderId {0};
    int m_qty;
    bool m_isBuy;
    std::time_t m_submitTime;
  
  public:
    Order(OrderBook& orderBook, int qty, bool isBuy);
    virtual void execute();
    std::string getSubmitTime();
    bool isBuy();
    int getOrderId();
    int getQty();
    virtual double getLimitPrice();
};
