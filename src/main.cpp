#include <iostream>
#include "CLIHandler.hpp"
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
  // Market Order Testing
  MarketOrder myMarketOrder(150, false);
  myMarketOrder.execute();

  // Limit Order Testing
  LimitOrder myLimitOrder(150, false, 1254.50);
  myLimitOrder.execute();

  // Stop Order Testing
  StopOrder myStopOrder(123, false, 123.50);
  myStopOrder.execute();

  // Stop Limit Order Testing
  StopLimitOrder myStopLimitOrder(123, false, 125, 100);
  myStopLimitOrder.execute();

  // Iceberg Order Testing
  IcebergOrder myIcebergOrder(1000, false, 100, 12.50);
  myIcebergOrder.execute();

  // Stop Limit Order Testing

  CLIHandler cliHandler;
  cliHandler.run();
}
