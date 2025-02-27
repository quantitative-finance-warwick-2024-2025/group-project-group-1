#include "OrderBook.hpp"
#include "Order.hpp"

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
void addOrder()
{}

// Method that handles matching of orders
void matchOrders()
{}

// Method that removes an order from the order book
void removeOrder()
{}

// Method that gets the best bid from the order book
Order getBestBid()
{}

// Method that gets the best ask from the order book
Order getBestAsk()
{}

// Method that gets the current market spread
double getMarketSpread()
{}

// Method that cancels an order
void cancelOrder()
{}

// Get the current snapshot of the book
std::string getBookSnapshot()
{}


