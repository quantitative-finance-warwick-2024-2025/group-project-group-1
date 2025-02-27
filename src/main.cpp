#include <iostream>
#include "CLIHandler.hpp"
#include "MarketOrder.hpp"
#include "LimitOrder.hpp"
#include "StopOrder.hpp"
#include "StopLimitOrder.hpp"
#include "IcebergOrder.hpp"
#include "Order.hpp"
#include <string>

int main()
{
  std::string timestamp = "2024-01-01";

  // Market Order Testing
  MarketOrder myMarketOrder(123, 150, false, timestamp);
  myMarketOrder.execute();

  // Limit Order Testing
  LimitOrder myLimitOrder(123, 150, false, timestamp, 1254.50);
  myLimitOrder.execute();

  // Stop Order Testing
  StopOrder myStopOrder(123, 123, false, timestamp, 123.50, false);
  myStopOrder.execute();

  // Stop Limit Order Testing
  StopLimitOrder myStopLimitOrder(123, 123, false, timestamp, 125.50, 125, false);
  myStopLimitOrder.execute();

  // Iceberg Order Testing
  IcebergOrder myIcebergOrder(123, 123, false, timestamp, 100, 1000, 123.30);
  myIcebergOrder.execute();

  // Stop Limit Order Testing

  CLIHandler cliHandler;
  cliHandler.run();
}
