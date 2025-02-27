#pragma once
#include "Order.hpp"
#include <string>
#include <vector>
#include <map>
#include <list>
#include <unordered_map>

class OrderBook
{
  private:
    std::map<double, std::list<Order*>, std::greater<double>> bids;
    std::map<double, std::list<Order*>> asks;
    std::unordered_map<int, Order*> orderIndex;
    std::vector<std::string> history;
    int nextOrderId;

  public:
    OrderBook();
    int generateOrderId();
    void addOrder(Order* order);
    void matchOrder();
    void removeOrder(Order* order);
    Order getBestBid();
    Order getBestAsk();
    double getMarketSpread();
    void cancelOrder(int orderId);
    std::string getBookSnapshot();
};
