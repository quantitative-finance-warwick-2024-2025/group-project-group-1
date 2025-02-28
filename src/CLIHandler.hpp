#pragma once
#include "OrderBook.hpp"
#include <string>
#include <vector>

class CLIHandler {
  private:
    OrderBook& m_orderBook;

  public:
    // Constructor
    CLIHandler(OrderBook& orderBook);

    bool isValidSide(std::string side);
    bool isValidDouble(std::string value);
    bool isValidInt(std::string value);
    // Print Help
    void displayHelp();
    // Parse Command Line Arguments
    std::vector<std::string> parseArguments(std::string command);
    // Handle orders
    void handleLimitOrder(std::vector<std::string> tokens);
    void handleMarketOrder(std::vector<std::string> tokens);
    void handleStopOrder(std::vector<std::string> tokens);
    void handleStopLimitOrder(std::vector<std::string> tokens);
    void handleIcebergOrder(std::vector<std::string> tokens);
    // Handle other commands
    void handleMarketSpread();
    void handleBest();

    // Run CLIHandler
    void run();
};
