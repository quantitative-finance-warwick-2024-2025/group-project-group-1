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
const std::vector<CommandInfo> commands = {
    {"LIMIT", "[BUY|SELL] <quantity> <price>", "Place a limit order"},
    {"MARKET", "[BUY|SELL] <quantity>", "Place a market order"},
    {"STOP", "[BUY|SELL] <quantity> <stop_price>", "Place a stop order"},
    {"STOPLIMIT", "[BUY|SELL] <quantity> <stop_price> <limit_price>", "Place a stop-limit order"},
    {"ICEBERG", "[BUY|SELL] <total_quantity> <displayed_qty> <price>", "Place an iceberg order"},
    {"VIEW", "<order_id>", "View an order by ID"},
    {"CANCEL", "<order_id>", "Cancel an order by ID"},
    {"BOOK", "", "Show the full order book"},
    {"DEPTH", "[N]", "Show market depth for N levels (default 5)"},
    {"SPREAD", "", "Show market spread"},
    {"BEST", "", "Show the best bid and ask"},
    {"HISTORY", "", "Show order history and executions"},
    {"HELP", "", "Display this help menu"},
    {"EXIT", "", "Exit the application"}};

// Invalid Numbers Error Message and Incorrect Formatting Error Message
const std::string INVALID_MSG =
    "Invalid values entered. Type 'HELP' to see the arguments of the command.";
const std::string FORMAT_ERROR_MSG =
    "Command is incorrectly formatted. Type 'HELP' to see the correct format of the command.";

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
        std::cout << "" << std::endl;
        std::vector<std::string> tokens = parseArguments(input);
        std::string command = tokens[0];

        if (command == "HELP") {
            displayHelp();
        } else if (tokens.size() == 4 && command == "LIMIT") {
            handleLimitOrder(tokens);
        } else if (tokens.size() == 3 && command == "MARKET") {
            handleMarketOrder(tokens);
        } else if (tokens.size() == 4 && command == "STOP") {
            // handleStopOrder(tokens);
        } else if (tokens.size() == 5 && command == "STOPLIMIT") {
            // handleStopLimitOrder(tokens);
        } else if (tokens.size() == 5 && command == "ICEBERG") {
            // handleIcebergOrder(tokens);
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
            std::cout
                << "Command not recognized. Type 'HELP' to see the list of available commands."
                << std::endl;
        }
        std::cout << "\n";
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

// // Handle Stop Orders
// void CLIHandler::handleStopOrder(std::vector<std::string> tokens) {
//     // Validation
//     if (!isValidSide(tokens[1]) || !isValidInt(tokens[2]) || !isValidDouble(tokens[3])) {
//         std::cout << FORMAT_ERROR_MSG << std::endl;
//         return;
//     }

//     // Everything is validated
//     double stop_price{std::stod(tokens[3])};
//     int qty{std::stoi(tokens[2])};
//     bool isBuy{tokens[1] == "BUY"};

//     // qty and stop price must be positive
//     if (qty <= 0 || stop_price < 0) {
//         std::cout << INVALID_MSG << std::endl;
//         return;
//     }

//     // Create Stop Order
//     StopOrder stopOrder(m_orderBook, qty, isBuy, stop_price);
//     stopOrder.execute();
// }

// // Handle Stop Limit Orders
// void CLIHandler::handleStopLimitOrder(std::vector<std::string> tokens) {
//     // Validation
//     if (!isValidSide(tokens[1]) || !isValidInt(tokens[2]) || !isValidDouble(tokens[3]) ||
//         !isValidDouble(tokens[4])) {
//         std::cout << FORMAT_ERROR_MSG << std::endl;
//         return;
//     }

//     // Everything is validated
//     double stop_price{std::stod(tokens[3])};
//     double limit_price{std::stod(tokens[4])};
//     int qty{std::stoi(tokens[2])};
//     bool isBuy{tokens[1] == "BUY"};

//     // stop_price, limit_price must be non-negative and qty positive
//     if (stop_price < 0 || limit_price < 0 || qty <= 0) {
//         std::cout << INVALID_MSG << std::endl;
//         return;
//     }

//     // Create StopLimitOrder
//     StopLimitOrder stopLimitOrder(m_orderBook, qty, isBuy, stop_price, limit_price);
//     stopLimitOrder.execute();
// }

// // Handle Iceberg Limit Orders
// void CLIHandler::handleIcebergOrder(std::vector<std::string> tokens) {
//     // Input Validation
//     if (!isValidSide(tokens[1]) || !isValidInt(tokens[2]) || !isValidInt(tokens[3]) ||
//         !isValidDouble(tokens[4])) {
//         std::cout << FORMAT_ERROR_MSG << std::endl;
//         return;
//     }

//     // Everything is validated
//     bool isBuy{tokens[1] == "BUY"};
//     int total_qty{std::stoi(tokens[2])};
//     double displayed_qty{std::stod(tokens[3])};
//     double limit_price{std::stod(tokens[4])};

//     // displayQty must be less than or equal to total_qty, and price must be positive
//     if (displayed_qty > total_qty || limit_price < 0 || total_qty <= 0 || displayed_qty <= 0) {
//         std::cout << INVALID_MSG << std::endl;
//         return;
//     }

//     // Create Iceberg Order
//     IcebergOrder icebergOrder(m_orderBook, total_qty, isBuy, displayed_qty, limit_price);
//     icebergOrder.execute();
// }

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

    std::cout << std::left << std::setw(col1width) << "Command" << std::left << std::setw(col2width)
              << "Arguments"
              << "Description" << '\n';

    // Print a straight line separator
    for (int i = 0; i < totalWidth; ++i) {
        std::cout << "-";
    }
    std::cout << "\n";

    for (const CommandInfo &command : commands) {
        std::cout << std::left << std::setw(col1width) << command.command << std::left
                  << std::setw(col2width) << command.arguments << command.description << '\n';
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
        std::cout << "   > No liquidity in the market. Unable to calculate market spread"
                  << std::endl;
    } else {
        std::cout << "   > Market Spread is $" << marketSpread << std::endl;
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
        Quantity availableBidQuantity =
            orderBook_.getBidQuantityAtLevel(bestBidOrderPointer->getPrice());
        std::cout << "Best Bid Offer: " << availableBidQuantity << " @ "
                  << bestBidOrderPointer->getPrice() << std::endl;
    }

    if (bestAskOrderPointer != nullptr) {
        Quantity availableAskQuantity =
            orderBook_.getAskQuantityAtLevel(bestAskOrderPointer->getPrice());

        std::cout << "Best Ask Offer: " << availableAskQuantity << " @ "
                  << bestAskOrderPointer->getPrice() << std::endl;
    }
}

// Cancel order by id
void CLIHandler::handleCancel(std::vector<std::string> tokens) {
    if (!isValidInt(tokens[1])) {
        std::cout << "OrderID must be an integer\n";
        return;
    }
    OrderId orderId = std::stoi(tokens[1]);
    orderBook_.removeOrder(orderId);
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

    // Print summmary of the order
    std::cout << "Summary of Order " << orderPointer->getOrderId() << std::endl;
    std::cout << "-----------------------------------------------\n";
    // Print order is buy or sell
    std::cout << "Order Type: " << (orderSide == Side::BUY ? "Buy Order" : "Sell Order")
              << std::endl;
    std::cout << "Limit Price: " << orderPointer->getPrice() << std::endl;
    std::cout << "Remaining Quantity: " << orderPointer->getRemainingQuantity() << std::endl;
    std::cout << "Filled Quantity: " << orderPointer->getFilledQuantity() << " out of requested "
              << orderPointer->getInitialQuantity() << std::endl;
    std::cout << "-----------------------------------------------\n";

    // std::cout << "Order Submitted
    //     at : " << viewOrder->getSubmitTime() << std::endl; std::cout <<
    //  "-----------------------------------------------\n";
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
