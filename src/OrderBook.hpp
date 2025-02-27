#pragma once
#include <string>
#include <vector>
#include <map>
#include <list>
#include <unordered_map>

class Order;

class OrderBook
{
  private:
    std::string marketName;
    std::map<double, std::list<Order*>, std::greater<double>> bids;
    std::map<double, std::list<Order*>> asks;
    std::unordered_map<int, Order*> orderIndex;
    std::vector<std::string> history;
    int currentOrderId { 0 };

  public:
    OrderBook();
    int generateOrderId();
    void addOrder();
    void matchOrder();
    void removeOrder();
    Order getBestBid();
    Order getBestAsk();
    double getMarketSpread();
    void cancelOrder();
    std::string getBookSnapshot();
};
