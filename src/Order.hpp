#pragma once
#include <string>

class Order
{
  protected:
    int m_orderId;
    int m_qty;
    bool m_isBuy;
    std::string m_timeStamp;
  
  public:
    Order(int orderId, int qty, bool isBuy, std::string& timeStamp);
    virtual void execute();
};
