#include "CLIHandler.hpp"
#include <iostream>
#include <iomanip>

// Struct to hold command information
struct CommandInfo
{
    std::string command;
    std::string arguments;
    std::string description;
};

// List of available commands
const std::vector<CommandInfo> commands = {
    {"LIMIT",     "[BUY|SELL] <price> <quantity>",      "Place a limit order"},
    {"MARKET",    "[BUY|SELL] <quantity>",              "Place a market order"},
    {"STOP",      "[BUY|SELL] <stop_price> <quantity>", "Place a stop order"},
    {"STOPLIMIT", "[BUY|SELL] <stop_price> <limit_price> <quantity>", "Place a stop-limit order"},
    {"ICEBERG",   "[BUY|SELL] <price> <total_quantity> <displayed_qty>", "Place an iceberg order"},
    {"CANCEL",    "<order_id>",                         "Cancel an order by ID"},
    {"BOOK",      "",                                   "Show the full order book"},
    {"DEPTH",     "[N]",                                "Show market depth for N levels (default 5)"},
    {"BEST",      "",                                   "Show the best bid and ask"},
    {"HISTORY",   "",                                   "Show order history and executions"},
    {"HELP",      "",                                   "Display this help menu"},
    {"EXIT",      "",                                   "Exit the application"}
};

// Constructor of the CLIHandler class
CLIHandler::CLIHandler() {
    std::cout << "\nLimit Order Book \n" << std::endl;
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
            std::cout << "LIMIT ORDER" << std::endl;
        }
        else if (tokens.size() == 3 && command == "MARKET")
        {
            std::cout << "MARKET ORDER" << std::endl;
        }
        else if (tokens.size() == 4 && command == "STOP")
        {
            std::cout << "STOP ORDER" << std::endl;
        }
        else if (tokens.size() == 5 && command == "STOPLIMIT")
        {
            std::cout << "STOP LIMIT ORDER" << std::endl;
        }
        else if (tokens.size() == 5 && command == "ICEBERG")
        {
            std::cout << "ICEBERG ORDER" << std::endl;
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