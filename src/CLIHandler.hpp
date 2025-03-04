#pragma once
#include "OrderBook.hpp"
#include <string>
#include <vector>

class CLIHandler {
  private:
    OrderBook &orderBook_;

  public:
    // Constructor
    CLIHandler(OrderBook &orderBook);
    bool isValidSide(std::string side);
    bool isValidDouble(std::string value);
    bool isValidInt(std::string value);
    void displayHelp();
    std::vector<std::string> parseArguments(std::string command);
    void handleLimitOrder(std::vector<std::string> tokens);
    void handleMarketOrder(std::vector<std::string> tokens);
    void handleStopOrder(std::vector<std::string> tokens);
    void handleStopLimitOrder(std::vector<std::string> tokens);
    void handleIcebergOrder(std::vector<std::string> tokens);
    void handleViewOrder(std::vector<std::string> tokens);
    void handleMarketSpread();
    void handleBest();
    void handleCancel(std::vector<std::string> tokens);
    void run();
};
