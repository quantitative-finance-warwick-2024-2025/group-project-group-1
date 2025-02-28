#pragma once
#include <string>
#include <vector>
#include <map>
#include <list>
#include <unordered_map>

class Order;
class LimitOrder;

class OrderBook
{
  private:
    std::string marketName;
    std::map<double, std::list<std::shared_ptr<Order>>, std::greater<double>> bids;
    std::map<double, std::list<std::shared_ptr<Order>>> asks;
    std::unordered_map<int, std::shared_ptr<Order>> orderIndex;
    std::vector<std::string> history;
    int currentOrderId { 0 };

  public:
    OrderBook();
    int generateOrderId();
    void addOrder();
    void addOrder(std::shared_ptr<LimitOrder> limitOrder);
    void matchOrders();
    void removeOrder();
    size_t getAskSize();
    size_t getBidSize();
    Order getBestBid();
    Order getBestAsk();
    std::shared_ptr<Order> getOrderById(int orderId);
    double getMarketSpread();
    void cancelOrder();
    void getBookSnapshot();
};
