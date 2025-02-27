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
  std::cout << orderBook.generateOrderId() << " " << orderBook.generateOrderId() << std::endl;

  // Market Order Testing
  MarketOrder myMarketOrder(orderBook, 150, false);
  myMarketOrder.execute();

  // Limit Order Testing
  LimitOrder myLimitOrder(orderBook, 150, false, 1254.50);
  myLimitOrder.execute();

  // Stop Order Testing
  StopOrder myStopOrder(orderBook, 123, false, 123.50);
  myStopOrder.execute();

  // Stop Limit Order Testing
  StopLimitOrder myStopLimitOrder(orderBook, 123, false, 125, 100);
  myStopLimitOrder.execute();

  // Iceberg Order Testing
  IcebergOrder myIcebergOrder(orderBook, 1000, false, 100, 12.50);
  myIcebergOrder.execute();

  // Stop Limit Order Testing

  CLIHandler cliHandler(orderBook);
  cliHandler.run();
}
