#define RESET "\033[0m"
#define BLACK "\033[30m"              /* Black */
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */

#include "CLIHandler.hpp"
#include "Order.hpp"
#include "OrderBook.hpp"
#include "Types.hpp"
#include <iomanip>
#include <iostream>

// Struct to hold command information
struct CommandInfo {
    std::string command;
    std::string arguments;
    std::string description;
};

// List of available commands
const std::vector<CommandInfo> commands = {{"LIMIT", "[BUY|SELL] <Quantity> <Limit Price>", "Place a limit order"},
                                           {"MARKET", "[BUY|SELL] <Quantity>", "Place a market order"},
                                           {"STOP", "[BUY|SELL] <Quantity> <Stop Price>", "Place a stop order"},
                                           {"STOPLIMIT", "[BUY|SELL] <Quantity> <Stop Price> <Limit Price>", "Place a stop-limit order"},
                                           {"VIEW", "<Order ID>", "View an order by ID"},
                                           {"CANCEL", "<Order ID>", "Cancel an order by ID"},
                                           {"BOOK", "", "Show the full order book"},
                                           //    {"DEPTH", "[N]", "Show market depth for N levels (default 5)"},
                                           {"SPREAD", "", "Show market spread"},
                                           {"BEST", "", "Show the best bid and ask"},
                                           //    {"HISTORY", "", "Show order history and executions"},
                                           {"HELP", "", "Display this help menu"},
                                           {"EXIT", "", "Exit the application"}};

// Invalid Numbers Error Message and Incorrect Formatting Error Message
const std::string INVALID_MSG =
    std::string(RED) + "[System] Invalid values entered. Type 'HELP' to see the arguments of the command." + std::string(RESET);
const std::string FORMAT_ERROR_MSG = std::string(RED) +
                                     "[System] Command is incorrectly formatted. Type 'HELP' to see the correct format of the command." +
                                     std::string(RESET);

// Constructor
CLIHandler::CLIHandler(OrderBook &orderBook) : orderBook_(orderBook) {}

// Check if the side is valid
bool CLIHandler::isValidSide(std::string side) { return (side == "BUY" || side == "SELL"); }

// Check if the value is a valid double
bool CLIHandler::isValidDouble(std::string value) {
    // Try to convert a string into a double variable
    try {
        std::stod(value);
        return true;
    } catch (...) {
        return false;
    }
}

// Check if the value is a valid integer
bool CLIHandler::isValidInt(std::string value) {
    // Try to convert a string into an integer variable
    try {
        std::stoi(value);
        return true;
    } catch (...) {
        return false;
    }
}

// Make the CLIHandler run
void CLIHandler::run() {
    std::string input;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        if (input.empty())
            continue;
        std::vector<std::string> tokens = parseArguments(input);
        std::string command = tokens[0];

        if (command == "HELP") {
            displayHelp();
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
        } else if (tokens.size() >= 1 && tokens.size() <= 2 && command == "DEPTH") {
            std::cout << "DEPTH" << std::endl;
        } else if (tokens.size() == 1 && command == "SPREAD") {
            handleMarketSpread();
        } else if (tokens.size() == 1 && command == "BEST") {
            handleBest();
        } else if (tokens.size() == 1 && command == "HISTORY") {
            std::cout << "HISTORY" << std::endl;
        } else if (command == "EXIT") {
            break;
        } else {
            std::cout << INVALID_MSG << std::endl;
        }
    }
}

// Handle Limit Orders
void CLIHandler::handleLimitOrder(std::vector<std::string> tokens) {
    // Validation
    if (!isValidSide(tokens[1]) || !isValidInt(tokens[2]) || !isValidDouble(tokens[3])) {
        std::cout << FORMAT_ERROR_MSG << std::endl;
        return;
    }

    // Everything is validated
    Price price{std::stod(tokens[3])};
    Quantity qty{std::stoi(tokens[2])};
    bool isBuy{tokens[1] == "BUY"};
    Side side = isBuy ? Side::BUY : Side::SELL;

    // Price and Qty must be positive
    if (price < 0 || qty <= 0) {
        std::cout << INVALID_MSG << std::endl;
        return;
    }

    orderBook_.submitOrder(Order::CreateLimitOrder(side, orderBook_.nextOrderId(), price, qty));
}

// Handle Market Orders
void CLIHandler::handleMarketOrder(std::vector<std::string> tokens) {
    // Validation
    if (!isValidSide(tokens[1]) || !isValidInt(tokens[2])) {
        std::cout << FORMAT_ERROR_MSG << std::endl;
        return;
    }

    // Everything is validated
    Quantity qty{std::stoi(tokens[2])};
    bool isBuy{tokens[1] == "BUY"};
    Side side = isBuy ? Side::BUY : Side::SELL;

    // Quantity must be positive
    if (qty <= 0) {
        std::cout << INVALID_MSG << std::endl;
        return;
    }

    // Create the MarketOrder
    orderBook_.submitOrder(Order::CreateMarketOrder(side, orderBook_.nextOrderId(), qty));
}

// Handle Stop Orders
void CLIHandler::handleStopOrder(std::vector<std::string> tokens) {
    // Validation
    if (!isValidSide(tokens[1]) || !isValidInt(tokens[2]) || !isValidDouble(tokens[3])) {
        std::cout << FORMAT_ERROR_MSG << std::endl;
        return;
    }

    // Everything is validated
    bool isBuy{tokens[1] == "BUY"};
    Side side = isBuy ? Side::BUY : Side::SELL;
    Quantity qty{std::stoi(tokens[2])};
    Price stopPrice{std::stod(tokens[3])};

    // qty and stop price must be positive
    if (qty <= 0 || stopPrice < 0) {
        std::cout << INVALID_MSG << std::endl;
        return;
    }

    // Create Stop Order
    Order stopOrder = Order::CreateStopOrder(side, orderBook_.nextOrderId(), stopPrice, qty);
    orderBook_.submitOrder(stopOrder);
}

// Handle Stop Limit Orders
void CLIHandler::handleStopLimitOrder(std::vector<std::string> tokens) {
    // Validation
    if (!isValidSide(tokens[1]) || !isValidInt(tokens[2]) || !isValidDouble(tokens[3]) || !isValidDouble(tokens[4])) {
        std::cout << FORMAT_ERROR_MSG << std::endl;
        return;
    }

    // Everything is validated
    Price stopPrice{std::stod(tokens[3])};
    Price limitPrice{std::stod(tokens[4])};
    Quantity qty{std::stoi(tokens[2])};
    bool isBuy{tokens[1] == "BUY"};
    Side side = isBuy ? Side::BUY : Side::SELL;

    // stop_price, limit_price must be non-negative and qty positive
    if (stopPrice < 0 || limitPrice < 0 || qty <= 0) {
        std::cout << INVALID_MSG << std::endl;
        return;
    }

    // Create StopLimitOrder
    Order stopLimitOrder = Order::CreateStopLimitOrder(side, orderBook_.nextOrderId(), stopPrice, limitPrice, qty);
    orderBook_.submitOrder(stopLimitOrder);
}

// Display the help menu for the user
void CLIHandler::displayHelp() {
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

// Display the market spread
void CLIHandler::handleMarketSpread() {
    double marketSpread = orderBook_.getMarketSpread();
    if (std::isnan(marketSpread)) {
        std::cout << RED << "[System] No liquidity in the market. Unable to calculate market spread" << RESET << std::endl;
    } else {
        std::cout << "[System] Market Spread is $" << marketSpread << std::endl;
    }
}

// Display best ask and best bid
void CLIHandler::handleBest() {
    OrderPointer bestBidOrderPointer = orderBook_.getBestBidOrder();
    OrderPointer bestAskOrderPointer = orderBook_.getBestAskOrder();

    if (bestBidOrderPointer == nullptr && bestAskOrderPointer == nullptr) {
        std::cout << "   > No liquidity in the market. \n";
    }

    if (bestBidOrderPointer != nullptr) {
        Quantity availableBidQuantity = orderBook_.getBidQuantityAtLevel(bestBidOrderPointer->getPrice());
        std::cout << "Best Bid Offer: " << availableBidQuantity << " @ " << bestBidOrderPointer->getPrice() << std::endl;
    }

    if (bestAskOrderPointer != nullptr) {
        Quantity availableAskQuantity = orderBook_.getAskQuantityAtLevel(bestAskOrderPointer->getPrice());

        std::cout << "Best Ask Offer: " << availableAskQuantity << " @ " << bestAskOrderPointer->getPrice() << std::endl;
    }
}

// Cancel order by id
void CLIHandler::handleCancel(std::vector<std::string> tokens) {
    if (!isValidInt(tokens[1])) {
        std::cout << "OrderID must be an integer\n";
        return;
    }
    OrderId orderId = std::stoi(tokens[1]);
    orderBook_.removeOrder(orderId, true);
}

// Display details on order by id
void CLIHandler::handleViewOrder(std::vector<std::string> tokens) {
    if (!isValidInt(tokens[1])) {
        std::cout << "OrderID must be an integer\n";
        return;
    }
    OrderId orderId = std::stoi(tokens[1]);
    OrderPointer orderPointer = orderBook_.viewOrder(orderId);
    if (orderPointer == nullptr) {
        std::cout << "Order with ID " << orderId << " not found\n";
        return;
    }

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

// Parse the arguments of the command to strip the whitespace
std::vector<std::string> CLIHandler::parseArguments(std::string command) {
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
