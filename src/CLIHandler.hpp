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

    // Validators
    bool isValidSide(Token side);
    bool isValidDouble(Token value);
    bool isValidInt(Token value);

    // Operational Methods
    void run();
    Tokens parseArguments(Command command);

    // Handler Methods
    void handleLimitOrder(Tokens tokens);
    void handleMarketOrder(Tokens tokens);
    void handleStopOrder(Tokens tokens);
    void handleStopLimitOrder(Tokens tokens);
    void handleViewOrder(Tokens tokens);
    void handleBest();
    void handleCancel(Tokens tokens);
    void handleMarketSpread();
    void handleHelp();
};
