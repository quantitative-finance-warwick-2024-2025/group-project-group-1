#include <iostream>
#include "CLIHandler.hpp"
#include "OrderBook.hpp"
#include "MarketOrder.hpp"
#include "LimitOrder.hpp"
#include "StopOrder.hpp"
#include "StopLimitOrder.hpp"
#include "IcebergOrder.hpp"
#include "Order.hpp"
#include <string>
#include <ctime>

int main()
{
  // Create OrderBook
  OrderBook orderBook;

  // Stop Limit Order Testing
  CLIHandler cliHandler(orderBook);
  cliHandler.run();
}
