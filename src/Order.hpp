#pragma once
#include <string>
#include <ctime>

class Order
{
  protected:
    int m_orderId;
    int m_qty;
    bool m_isBuy;
    std::time_t m_submitTime;
  
  public:
    Order(int qty, bool isBuy);
    virtual void execute();
    std::string getSubmitTime();
};
