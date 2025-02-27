#include "CLIHandler.hpp"
#include <iostream>
#include <iomanip>
#include "MarketOrder.hpp"
#include "LimitOrder.hpp"
#include "StopOrder.hpp"
#include "StopLimitOrder.hpp"
#include "IcebergOrder.hpp"

// Struct to hold command information
struct CommandInfo
{
    std::string command;
    std::string arguments;
    std::string description;
};

// List of available commands
const std::vector<CommandInfo> commands = {
    {"LIMIT",     "[BUY|SELL] <quantity> <price>",                          "Place a limit order"},
    {"MARKET",    "[BUY|SELL] <quantity>",                                  "Place a market order"},
    {"STOP",      "[BUY|SELL] <quantity> <stop_price>",                     "Place a stop order"},
    {"STOPLIMIT", "[BUY|SELL] <quantity> <stop_price> <limit_price>",       "Place a stop-limit order"},
    {"ICEBERG",   "[BUY|SELL] <total_quantity> <displayed_qty> <price>",    "Place an iceberg order"},
    {"CANCEL",    "<order_id>",                                             "Cancel an order by ID"},
    {"BOOK",      "",                                                       "Show the full order book"},
    {"DEPTH",     "[N]",                                                    "Show market depth for N levels (default 5)"},
    {"BEST",      "",                                                       "Show the best bid and ask"},
    {"HISTORY",   "",                                                       "Show order history and executions"},
    {"HELP",      "",                                                       "Display this help menu"},
    {"EXIT",      "",                                                       "Exit the application"}
};

// Constructor of the CLIHandler class
CLIHandler::CLIHandler() {
    std::cout << "\nLimit Order Book \n" << std::endl;
}

// Check if the side is valid
bool CLIHandler::isValidSide(std::string &side)
{
    return (side == "BUY" || side == "SELL");
}

// Check if the value is a valid double
bool CLIHandler::isValidDouble(std::string &value)
{
    try
    {
        std::stod(value);
        return true;
    }
    catch (std::exception &e)
    {
        return false;
    }
}

// Check if the value is a valid integer
bool CLIHandler::isValidInt(std::string &value)
{
    try
    {
        std::stoi(value);
        return true;
    }
    catch (std::exception &e)
    {
        return false;
    }
}

// Make the CLIHandler run
void CLIHandler::run() 
{
    std::string input;

    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input.empty()) continue;

        std::cout << "" << std::endl;

        std::vector<std::string> tokens = parseArguments(input);
        std::string command = tokens[0];

        if (command == "HELP")
        {
            displayHelp();
        } 
        else if (tokens.size() == 4 && command == "LIMIT")
        {
            handleLimitOrder(tokens);
        }
        else if (tokens.size() == 3 && command == "MARKET")
        {
            handleMarketOrder(tokens);
        }
        else if (tokens.size() == 4 && command == "STOP")
        {
            handleStopOrder(tokens);
        }
        else if (tokens.size() == 5 && command == "STOPLIMIT")
        {
            handleStopLimitOrder(tokens);
        }
        else if (tokens.size() == 5 && command == "ICEBERG")
        {
            handleIcebergOrder(tokens);
        }
        else if (tokens.size() == 2 && command == "CANCEL")
        {
            std::cout << "CANCEL ORDER" << std::endl;
        }
        else if (tokens.size() == 1 && command == "BOOK")
        {
            std::cout << "BOOK" << std::endl;
        }
        else if (tokens.size() >= 1 && tokens.size() <= 2 && command == "DEPTH")
        {
            std::cout << "DEPTH" << std::endl;
        }
        else if (tokens.size() == 1 && command == "BEST")
        {
            std::cout << "BEST" << std::endl;
        }
        else if (tokens.size() == 1 && command == "HISTORY")
        {
            std::cout << "HISTORY" << std::endl;
        }
        else if(command == "EXIT")
        {
            break;
        }
        else 
        {
            std::cout << "Command not recognized. Type 'HELP' to see the list of available commands." << std::endl;
        }

        std::cout << "\n";
    }
}

// Handle Limit Orders
void CLIHandler::handleLimitOrder(std::vector<std::string> &tokens)
{
  // Validate Side
  if (!isValidSide(tokens[1]))
  {
    std::cout << "Second argument must be BUY or SELL" << std::endl;
    return;
  }

  // Validate quantity is int
  if (!isValidInt(tokens[2]))
  {
    std::cout << "Quantity must be an integer" << std::endl;
    return;
  }

  // Validate price is double
  if (!isValidDouble(tokens[3]))
  {
    std::cout << "Price must be a double" << std::endl;
    return;
  }

  // Everything is validated
  double price { std::stod(tokens[3]) };
  int qty { std::stoi(tokens[2]) };
  bool isBuy { tokens[1] == "BUY" };
  std::string timeStamp { "2024-01-01" };

  // Create LimitOrder
  LimitOrder limitOrder(123, qty, isBuy, timeStamp, price);
  limitOrder.execute();
}

// Handle Market Orders
void CLIHandler::handleMarketOrder(std::vector<std::string> &tokens)
{
  // Validate Side
  if (!isValidSide(tokens[1]))
  {
    std::cout << "Second argument must be BUY or SELL" << std::endl;
    return;
  }

  // Validate quantity is int
  if (!isValidInt(tokens[2]))
  {
    std::cout << "Quantity must be an integer" << std::endl;
    return;
  }

  // Everything is validated
  int qty { std::stoi(tokens[2]) };
  bool isBuy { tokens[1] == "BUY" };
  std::string timeStamp { "2024-01-01" };

  // Create the MarketOrder
  MarketOrder marketOrder(123, qty, isBuy, timeStamp);
  marketOrder.execute();
}

// Handle Stop Orders
void CLIHandler::handleStopOrder(std::vector<std::string> &tokens)
{
  // Validate Side
  if (!isValidSide(tokens[1]))
  {
    std::cout << "Second argument must be BUY or SELL" << std::endl;
    return;
  }

  // Validate quantity is int
  if (!isValidInt(tokens[2]))
  {
    std::cout << "Quantity must be an integer" << std::endl;
    return;
  }

  // Validate stop price is double
  if (!isValidDouble(tokens[3]))
  {
    std::cout << "Stop Price must be a double" << std::endl;
    return;
  }

  // Everything is validated
  double stop_price { std::stod(tokens[3]) };
  int qty { std::stoi(tokens[2]) };
  bool isBuy { tokens[1] == "BUY" };
  std::string timeStamp { "2024-01-01" };  

  // Create Stop Order
  StopOrder stopOrder(123, qty, isBuy, timeStamp, stop_price, false);
  stopOrder.execute();
}

// Handle Stop Limit Orders
void CLIHandler::handleStopLimitOrder(std::vector<std::string> &tokens) 
{
  // Validate Side
  if (!isValidSide(tokens[1]))
  {
    std::cout << "Second argument must be BUY or SELL" << std::endl;
    return;
  }

  // Validate quantity is int
  if (!isValidInt(tokens[2]))
  {
    std::cout << "Quantity must be an integer" << std::endl;
    return;
  }

  // Validate stop price is double
  if (!isValidDouble(tokens[3]))
  {
    std::cout << "Stop Price must be a double" << std::endl;
    return;
  }

  // Validate limit price is double
  if (!isValidDouble(tokens[4]))
  {
    std::cout << "Limit Price must be a double" << std::endl;
    return;
  }

  // Everything is validated
  double stop_price { std::stod(tokens[3]) };
  double limit_price { std::stod(tokens[4]) };
  int qty { std::stoi(tokens[2]) };
  bool isBuy { tokens[1] == "BUY" };
  std::string timeStamp { "2024-01-01" };
  
  // Create StopLimitOrder
  StopLimitOrder stopLimitOrder(123, qty, isBuy, timeStamp, stop_price, limit_price, false);
  stopLimitOrder.execute();
}

// Handle Iceberg Limit Orders
void CLIHandler::handleIcebergOrder(std::vector<std::string> &tokens) 
{
  // Validate Side
  if (!isValidSide(tokens[1]))
  {
    std::cout << "Second argument must be BUY or SELL" << std::endl;
    return;
  }

  // Validate total quantity is int
  if (!isValidInt(tokens[2]))
  {
    std::cout << "Total Quantity must be an integer" << std::endl;
    return;
  }

  // Validate displayed quantity is int
  if (!isValidInt(tokens[3]))
  {
     std::cout << "Displayed Quantity must be an integer" << std::endl;
    return;
  }

  // Validate price is double
  if (!isValidDouble(tokens[4]))
  {
    std::cout << "Limit Price must be an integer" << std::endl;
    return;
  }

  // Everything is validated
  bool isBuy { tokens[1] == "BUY" };
  int total_qty { std::stoi(tokens[2]) };
  double displayed_qty { std::stod(tokens[3]) };
  double limit_price { std::stod(tokens[4]) };
  std::string timeStamp { "2024-01-01" };

  // Create Iceberg Order
  IcebergOrder icebergOrder(123, total_qty, isBuy, timeStamp, displayed_qty, total_qty - displayed_qty, limit_price); 
  icebergOrder.execute();
}

// Display the help menu for the user
void CLIHandler::displayHelp() 
{
    std::cout << "Find listed below the list of commands to interact with the market." << std::endl;

    std::cout << "" << std::endl;

    const int col1width = 15;
    const int col2width = 55;

    int totalWidth = col1width + col2width + 45;
    for (int i = 0; i < totalWidth; ++i) {
        std::cout << "-";
    }
    std::cout << "\n";

    std::cout   << std::left << std::setw(col1width) << "Command"
                << std::left << std::setw(col2width) << "Arguments"
                << "Description" << '\n';

    // Print a straight line separator
    for (int i = 0; i < totalWidth; ++i) {
        std::cout << "-";
    }
    std::cout << "\n";

    for (const CommandInfo& command : commands)
    {
        std::cout   << std::left << std::setw(col1width) << command.command
                    << std::left << std::setw(col2width) << command.arguments
                    << command.description << '\n';
    }

    // Print a straight line separator
    for (int i = 0; i < totalWidth; ++i) {
        std::cout << "-";
    }

    std::cout << "\n";
}

// Parse the arguments of the command to strip the whitespace
std::vector<std::string> CLIHandler::parseArguments(std::string &command) 
{
    // Vector containing the command split by whitespace
    std::vector<std::string> parsedTokens;
    std::string currentToken;

    // For each character in the string captured
    for (char c : command)
    {
        // If the character is a whitespace and the currentToken is not empty, then we have a token
        if (c == ' ' && !currentToken.empty())
        {
            parsedTokens.push_back(currentToken);
            currentToken.clear();
        }
        else if (c == ' ' && currentToken.empty())
        {
            continue;
        }
        else 
        {
            // Otherwise, we have a chracter that makes up a token, so add it to the currentToken
            currentToken.push_back(c);
        }
    }

    // We have reached the end of string, so add the last token to the parsedTokens vector
    if (!currentToken.empty())
    {
        parsedTokens.push_back(currentToken);
    }

    return parsedTokens;
}
