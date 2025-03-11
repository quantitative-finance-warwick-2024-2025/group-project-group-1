#include "CLIHandler.hpp"
#include "Order.hpp"
#include "OrderBook.hpp"
#include "Types.hpp"
#include <iomanip>
#include <iostream>

// List of available commands
const Commands commands = {{"LIMIT", "[BUY|SELL] <Quantity> <Limit Price>", "Place a limit order"},
                           {"MARKET", "[BUY|SELL] <Quantity>", "Place a market order"},
                           {"STOP", "[BUY|SELL] <Quantity> <Stop Price>", "Place a stop order"},
                           {"STOPLIMIT", "[BUY|SELL] <Quantity> <Stop Price> <Limit Price>", "Place a stop-limit order"},
                           {"VIEW", "<Order ID>", "View an order by ID"},
                           {"CANCEL", "<Order ID>", "Cancel an order by ID"},
                           {"BOOK", "", "Show the full order book"},
                           {"SPREAD", "", "Show market spread"},
                           {"BEST", "", "Show the best bid and ask"},
                           {"HELP", "", "Display this help menu"},
                           {"EXIT", "", "Exit the application"}};

// Invalid Numbers Error Message and Incorrect Formatting Error Message
const std::string INVALID_MSG =
    std::string(RED) + "[System] Invalid values entered. Type 'HELP' to see the arguments of the command." + std::string(RESET);

const std::string FORMAT_ERROR_MSG = std::string(RED) +
                                     "[System] Command is incorrectly formatted. Type 'HELP' to see the correct format of the command." +
                                     std::string(RESET);

///////////////////////////////////////////////////////////////
// Constructors
///////////////////////////////////////////////////////////////

CLIHandler::CLIHandler(OrderBook &orderBook) : orderBook_(orderBook) {}

///////////////////////////////////////////////////////////////
// Validation Methods
///////////////////////////////////////////////////////////////

// Check if the side is valid
bool CLIHandler::isValidSide(Token side) { return (side == "BUY" || side == "SELL"); }

// Check if the value is a valid double
bool CLIHandler::isValidDouble(Token value) {
    // Try to convert a string into a double variable
    try {
        std::stod(value);
        return true;
    } catch (...) {
        return false;
    }
}

// Check if the value is a valid integer
bool CLIHandler::isValidInt(Token value) {
    // Try to convert a string into an integer variable
    try {
        std::stoi(value);
        return true;
    } catch (...) {
        return false;
    }
}

///////////////////////////////////////////////////////////////
// Handler Methods
///////////////////////////////////////////////////////////////

// Make the CLIHandler run
void CLIHandler::run() {
    std::string input;

    while (true) {
        std::cout << BLUE << "> " << RESET;
        std::getline(std::cin, input);
        if (input.empty())
            continue;
        std::vector<std::string> tokens = parseArguments(input);
        std::string command = tokens[0];

        if (tokens.size() == 1 && command == "HELP") {
            handleHelp();
        } else if (tokens.size() == 4 && command == "LIMIT") {
            handleLimitOrder(tokens);
        } else if (tokens.size() == 3 && command == "MARKET") {
            handleMarketOrder(tokens);
        } else if (tokens.size() == 4 && command == "STOP") {
            handleStopOrder(tokens);
        } else if (tokens.size() == 5 && command == "STOPLIMIT") {
            handleStopLimitOrder(tokens);
        } else if (tokens.size() == 2 && command == "CANCEL") {
            handleCancel(tokens);
        } else if (tokens.size() == 2 && command == "VIEW") {
            handleViewOrder(tokens);
        } else if (tokens.size() == 1 && command == "BOOK") {
            orderBook_.getBookSnapshot();
        } else if (tokens.size() == 1 && command == "SPREAD") {
            handleMarketSpread();
        } else if (tokens.size() == 1 && command == "BEST") {
            handleBest();
        } else if (command == "EXIT") {
            break;
        } else {
            std::cout << INVALID_MSG << std::endl;
        }
    }
}

// Parse the arguments of the command to strip the whitespace
std::vector<std::string> CLIHandler::parseArguments(Command command) {
    // Vector containing the command split by whitespace
    std::vector<std::string> parsedTokens;
    std::string currentToken;

    // For each character in the string captured
    for (char c : command) {
        // If the character is a whitespace and the currentToken is not empty, then we have a token
        if (c == ' ' && !currentToken.empty()) {
            parsedTokens.push_back(currentToken);
            currentToken.clear();
        } else if (c == ' ' && currentToken.empty()) {
            continue;
        } else {
            // Otherwise, we have a chracter that makes up a token, so add it to the currentToken
            currentToken.push_back(c);
        }
    }

    // We have reached the end of string, so add the last token to the parsedTokens vector
    if (!currentToken.empty()) {
        parsedTokens.push_back(currentToken);
    }

    return parsedTokens;
}

///////////////////////////////////////////////////////////////
// Handler Methods
///////////////////////////////////////////////////////////////

// Handle Limit Orders
void CLIHandler::handleLimitOrder(Tokens tokens) {
    // Validation
    if (!isValidSide(tokens[1]) || !isValidInt(tokens[2]) || !isValidDouble(tokens[3])) {
        std::cout << FORMAT_ERROR_MSG << std::endl;
        return;
    }

    // Get the quantities
    Price limitPrice = std::stod(tokens[3]);
    Quantity initialQuantity = std::stoi(tokens[2]);
    bool isBuy = tokens[1] == "BUY";
    Side side = isBuy ? Side::BUY : Side::SELL;

    // Limit Price and Initial Quantity must be positive
    if (limitPrice < 0 || initialQuantity <= 0) {
        std::cout << INVALID_MSG << std::endl;
        return;
    }

    // Submit the order
    orderBook_.submitOrder(Order::CreateLimitOrder(side, orderBook_.nextOrderId(), limitPrice, initialQuantity));
}

// Handle Market Orders
void CLIHandler::handleMarketOrder(Tokens tokens) {
    // Validation
    if (!isValidSide(tokens[1]) || !isValidInt(tokens[2])) {
        std::cout << FORMAT_ERROR_MSG << std::endl;
        return;
    }

    // Everything is validated
    Quantity initialQuantity = std::stoi(tokens[2]);
    bool isBuy = (tokens[1] == "BUY");
    Side side = isBuy ? Side::BUY : Side::SELL;

    // Initial Quantity must be positive
    if (initialQuantity <= 0) {
        std::cout << INVALID_MSG << std::endl;
        return;
    }

    // Submit the market order
    orderBook_.submitOrder(Order::CreateMarketOrder(side, orderBook_.nextOrderId(), initialQuantity));
}

// Handle Stop Orders (Stop-Market)
void CLIHandler::handleStopOrder(Tokens tokens) {
    // Validation
    if (!isValidSide(tokens[1]) || !isValidInt(tokens[2]) || !isValidDouble(tokens[3])) {
        std::cout << FORMAT_ERROR_MSG << std::endl;
        return;
    }

    // Everything is validated
    bool isBuy = tokens[1] == "BUY";
    Side side = isBuy ? Side::BUY : Side::SELL;
    Quantity initialQuantity = std::stoi(tokens[2]);
    Price stopPrice = std::stod(tokens[3]);

    // Initial quantity and stop price must be positive
    if (initialQuantity <= 0 || stopPrice < 0) {
        std::cout << INVALID_MSG << std::endl;
        return;
    }

    // Submit stop-market order
    orderBook_.submitOrder(Order::CreateStopOrder(side, orderBook_.nextOrderId(), stopPrice, initialQuantity));
}

// Handle Stop Limit Orders
void CLIHandler::handleStopLimitOrder(Tokens tokens) {
    // Validation
    if (!isValidSide(tokens[1]) || !isValidInt(tokens[2]) || !isValidDouble(tokens[3]) || !isValidDouble(tokens[4])) {
        std::cout << FORMAT_ERROR_MSG << std::endl;
        return;
    }

    // Everything is validated
    Price stopPrice = std::stod(tokens[3]);
    Price limitPrice = std::stod(tokens[4]);
    Quantity initialQuantity = std::stoi(tokens[2]);
    bool isBuy = tokens[1] == "BUY";
    Side side = isBuy ? Side::BUY : Side::SELL;

    // stopPrice, limitPrice must be non-negative and initialQuantity positive
    if (stopPrice < 0 || limitPrice < 0 || initialQuantity <= 0) {
        std::cout << INVALID_MSG << std::endl;
        return;
    }

    // Submit stop-limit order to the book
    orderBook_.submitOrder(Order::CreateStopLimitOrder(side, orderBook_.nextOrderId(), stopPrice, limitPrice, initialQuantity));
}

// Display the market spread
void CLIHandler::handleMarketSpread() {
    // Get the market spread from the orderbook
    double marketSpread = orderBook_.getMarketSpread();
    if (std::isnan(marketSpread)) {
        std::cout << RED << "[System] No liquidity in the market. Unable to calculate market spread" << RESET << std::endl;
    } else {
        std::cout << "[System] Market Spread is $" << marketSpread << std::endl;
    }
}

// Display best ask and best bid
void CLIHandler::handleBest() {
    // Get best bid and best ask
    OrderPointer bestBidOrderPointer = orderBook_.getBestBidOrder();
    OrderPointer bestAskOrderPointer = orderBook_.getBestAskOrder();

    // If no best bid and no best ask -> no orders in the market
    if (bestBidOrderPointer == nullptr && bestAskOrderPointer == nullptr) {
        std::cout << RED << "[System] No liquidity in the market." << RESET << std::endl;
    }

    // If bids book is not empty, show the best ask
    if (bestBidOrderPointer != nullptr) {
        // Must get all available quantity at this price
        Quantity availableBidQuantity = orderBook_.getBidQuantityAtLevel(bestBidOrderPointer->getPrice());
        std::cout << "[System] Best Bid Offer: " << availableBidQuantity << " @ " << bestBidOrderPointer->getPrice() << std::endl;
    }

    // If asks book is not empty, show the best ask
    if (bestAskOrderPointer != nullptr) {
        // Must get all available quantity at this price
        Quantity availableAskQuantity = orderBook_.getAskQuantityAtLevel(bestAskOrderPointer->getPrice());
        std::cout << "[System] Best Ask Offer: " << availableAskQuantity << " @ " << bestAskOrderPointer->getPrice() << std::endl;
    }
}

// Cancel order by id
void CLIHandler::handleCancel(Tokens tokens) {
    // Validate
    if (!isValidInt(tokens[1])) {
        std::cout << "Order ID must be an integer\n";
        return;
    }

    // Store orderId in typed variable
    OrderId orderId = std::stoi(tokens[1]);

    // Handle removal of order - will print the outcome
    orderBook_.removeOrder(orderId, true);
}

// Display details on order by id
void CLIHandler::handleViewOrder(Tokens tokens) {
    // Validate order ID
    if (!isValidInt(tokens[1])) {
        std::cout << "OrderID must be an integer\n";
        return;
    }

    // Store variables in typed variables
    OrderId orderId = std::stoi(tokens[1]);
    OrderPointer orderPointer = orderBook_.getOrder(orderId);

    // Check if the order exists
    if (orderPointer == nullptr) {
        std::cout << "Order with ID " << orderId << " not found\n";
        return;
    }

    // Get order details
    Side orderSide = orderPointer->getSide();
    OrderType orderType = orderPointer->getOrderType();
    IsTriggered isTriggered = orderPointer->getIsTriggered();
    bool isStopMarket = orderType == OrderType::STOP_MARKET;
    bool isStopLimit = orderType == OrderType::STOP_LIMIT;
    bool isLimit = orderType == OrderType::LIMIT;

    // Print summmary of the order
    std::cout << "Summary of Order " << orderPointer->getOrderId() << std::endl;
    std::cout << "----------------------------------------------------\n";

    // Print order is buy or sell
    std::cout << "Order Type: " << (isStopMarket ? "Stop Market " : "") << (isStopLimit ? "Stop Limit " : "")
              << (orderSide == Side::BUY ? "Buy Order" : "Sell Order") << (isTriggered ? " (Triggered)" : " (Not Triggered)") << std::endl;

    // Show the Limit Price
    if (isLimit || isStopLimit) {
        std::cout << "Limit Price: " << orderPointer->getPrice() << std::endl;
    }

    // Show the Stop Price
    if (isStopMarket || isStopLimit) {
        std::cout << "Stop Price: " << orderPointer->getStopPrice() << std::endl;
    }

    std::cout << "Remaining Quantity: " << orderPointer->getRemainingQuantity() << std::endl;
    std::cout << "Filled Quantity: " << orderPointer->getFilledQuantity() << " out of requested " << orderPointer->getInitialQuantity()
              << std::endl;

    std::cout << "----------------------------------------------------\n";
}

// Display the help menu for the user
void CLIHandler::handleHelp() {
    std::cout << "Find listed below the list of commands to interact with the market." << std::endl;

    std::cout << "" << std::endl;

    const int col1width = 15;
    const int col2width = 55;

    int totalWidth = col1width + col2width + 45;
    for (int i = 0; i < totalWidth; ++i) {
        std::cout << "-";
    }
    std::cout << "\n";

    std::cout << std::left << std::setw(col1width) << "Command" << std::left << std::setw(col2width) << "Arguments"
              << "Description" << '\n';

    // Print a straight line separator
    for (int i = 0; i < totalWidth; ++i) {
        std::cout << "-";
    }
    std::cout << "\n";

    for (const CommandInfo &command : commands) {
        std::cout << std::left << std::setw(col1width) << command.command << std::left << std::setw(col2width) << command.arguments
                  << command.description << '\n';
    }

    // Print a straight line separator
    for (int i = 0; i < totalWidth; ++i) {
        std::cout << "-";
    }

    std::cout << "\n";
}
