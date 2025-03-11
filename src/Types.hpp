#pragma once
#include <limits>
#include <list>
#include <map>
#define RESET "\033[0m"
#define RED "\033[31m"     /* Red */
#define GREEN "\033[32m"   /* Green */
#define YELLOW "\033[33m"  /* Yellow */
#define BLUE "\033[34m"    /* Blue */
#define MAGENTA "\033[35m" /* Magenta */
#define CYAN "\033[36m"    /* Cyan */

class Order;

enum class Side { BUY, SELL };
enum class OrderType { MARKET, LIMIT, STOP_MARKET, STOP_LIMIT };

struct CommandInfo {
    std::string command;
    std::string arguments;
    std::string description;
};
using Commands = std::vector<CommandInfo>;

using Price = double;
using Quantity = int;
using OrderId = int;
using Time = std::time_t;
using IsTriggered = bool;

// Pointers - Shared Pointers
using OrderPointer = std::shared_ptr<Order>;
using OrderPointers = std::list<OrderPointer>;

// Book Types
using Bids = std::map<Price, OrderPointers, std::greater<Price>>;
using Asks = std::map<Price, OrderPointers, std::less<Price>>;
using StopBids = std::map<Price, OrderPointers, std::less<Price>>;
using StopAsks = std::map<Price, OrderPointers, std::greater<Price>>;

using Orders = std::unordered_map<OrderId, OrderPointer>;

using Tokens = std::vector<std::string>;
using Token = std::string;
using Command = std::string;

// General constants used
struct Constants {
    static constexpr Price NoPrice = std::numeric_limits<Price>::quiet_NaN();
};
