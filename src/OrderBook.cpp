#include "OrderBook.hpp"
#include "LimitOrder.hpp"
#include "Order.hpp"
#include <iostream>
#include <list>
#include <iomanip>

// Create constructor
OrderBook::OrderBook()
  :
  marketName("LOB")
{}

// Method that generates an orderId
int OrderBook::generateOrderId() 
{
  currentOrderId++;
  return currentOrderId;
}

// Method that add orders to the book
void OrderBook::addOrder()
{}

// Add Limit Order to the Book
void OrderBook::addOrder(std::shared_ptr<LimitOrder> limitOrder)
{
  // Get the details of the limitOrder
  double limitPrice = limitOrder->getLimitPrice();
  int orderId = limitOrder->getOrderId();
  orderIndex[orderId] = limitOrder;

  // Submit the order to the correct side of the order book
  if (limitOrder->isBuy())
  {
    // If is buy order - add to the bid side
    bids[limitPrice].push_back(limitOrder);
    // std::cout << bids[limitPrice].front()->getOrderId() << std::endl;
    std::cout << "Buy Limit Order added to the bid side" << std::endl;
  }
  else
  {
    // If is sell order - add to the ask side
    asks[limitPrice].push_back(limitOrder);
    // std::cout << asks[limitPrice].front()->getOrderId() << std::endl;
    std::cout << "Sell Limit Order added to the ask side" << std::endl;
  }
}

// Method that handles matching of orders
void OrderBook::matchOrders()
{}

// Method that removes an order from the order book
void OrderBook::removeOrder()
{}

// Method that returns the size of the ask side
size_t OrderBook::getAskSize()
{
  size_t askSideSize = asks.size();
  return askSideSize;
}

// Method that returns the size of the bid size
size_t OrderBook::getBidSize()
{
  size_t bidSideSize = bids.size();
  return bidSideSize;
}

// Method that gets the best bid from the order book
Order OrderBook::getBestBid()
{}

// Method that gets the best ask from the order book
Order OrderBook::getBestAsk()
{}

// Method that views an order by id
std::shared_ptr<Order> OrderBook::getOrderById(int orderId)
{
  // Check if orderId exists in orderIndex
  if (orderIndex.find(orderId) == orderIndex.end())
  {
    return nullptr;
  }
  return orderIndex[orderId];
}

// Method that gets the current market spread
double OrderBook::getMarketSpread()
{
  // Check there exists at least one ask and at least one bid
  double spread = 0;
  if (bids.size() == 0 || asks.size() == 0)
  {
    spread = -1;
  }
  else 
  {
    double lowestAsk = bids.begin()->first;
    double lowestBid = asks.begin()->first;
    spread = lowestAsk - lowestBid;
  }

  return spread;
}

// Method that cancels an order
void OrderBook::cancelOrder()
{}

// Get the current snapshot of the book
void OrderBook::getBookSnapshot()
{
  // std::shared_ptr<Order> bestBid = bids.begin()->second.front();
  // // // print the first price in bids
  // std::cout << "Best Bid: " << bestBid->getOrderId() << std::endl;
  // std::cout << "Best Bid: " << bestBid->getSubmitTime() << std::endl;
  // for (std::pair<double, std::list<std::shared_ptr<Order>>> pair : bids)
  // {
  //   double priceLevel = pair.first;
  //   std::list<std::shared_ptr<Order>> orders = pair.second;
  //   std::cout << "Price Level = " << priceLevel << " ("<< orders.size() << " order(s)):\n";
  //   for (std::shared_ptr<Order> order : orders) 
  //   {
  //     std::cout << "  Order ID: " << order->getOrderId() << " for " << order->getQty() << '\n';
  //   }
  // }

  // for (std::pair<double, std::list<std::shared_ptr<Order>>> pair : asks)
  // {
  //   double priceLevel = pair.first;
  //   std::list<std::shared_ptr<Order>> orders = pair.second;
  //   std::cout << "Price Level = " << priceLevel << " ("<< orders.size() << " order(s)):\n";
  //   for (std::shared_ptr<Order> order : orders) 
  //   {
  //     std::cout << "  Order ID: " << order->getOrderId() << " for " << order->getQty() << '\n';
  //   }
  // }
  // Define column widths
  const int priceWidth = 10;
  const int qtyWidth = 8;

  std::cout << "This is the current state of the book:\n\n";

  // Print header
  std::cout << std::setw(priceWidth) << "Asks"
            << std::setw(qtyWidth) << "Qty"
            << "  |  "
            << std::setw(priceWidth) << "Bids"
            << std::setw(qtyWidth) << "Qty"
            << '\n';

  std::cout << "------------------------------------------\n";

  // Iterators for bids and asks
  auto bidIt = bids.begin();
  auto askIt = asks.begin();

  while (bidIt != bids.end() || askIt != asks.end())
  {
    // Print Ask side (Sell Orders)
    if (askIt != asks.end())
    {
      double askPrice = askIt->first;
      int askQty = 0;
      for (const auto& order : askIt->second) askQty += order->getQty();  // Sum total quantity at this price level
      std::cout << std::setw(priceWidth) << askPrice << std::setw(qtyWidth) << askQty;
      ++askIt;
    }
    else
    {
      // Print empty spaces if there are no more asks
      std::cout << std::setw(priceWidth) << " " << std::setw(qtyWidth) << " ";
    }

    std::cout << "  |  ";

    // Print Bid side (Buy Orders)
    if (bidIt != bids.end())
    {
      double bidPrice = bidIt->first;
      int bidQty = 0;
      for (const auto& order : bidIt->second) bidQty += order->getQty();  // Sum total quantity at this price level
      std::cout << std::setw(priceWidth) << bidPrice << std::setw(qtyWidth) << bidQty;
      ++bidIt;
    }
    else
    {
      // Print empty spaces if there are no more bids
      std::cout << std::setw(priceWidth) << " " << std::setw(qtyWidth) << " ";
    }

    std::cout << '\n';  // Move to the next row
  }

  std::cout << "------------------------------------------\n";

}