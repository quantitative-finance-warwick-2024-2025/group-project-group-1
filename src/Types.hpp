#pragma once
#include <limits>
#include <list>
#include <map>

class Order;

enum class Side { BUY, SELL };
enum class OrderType { MARKET, LIMIT, STOP_MARKET, STOP_LIMIT };

using Price = double;
using Quantity = int;
using OrderId = int;
using Time = std::time_t;

using OrderPointer = std::shared_ptr<Order>;
using OrderPointers = std::list<OrderPointer>;
using Bids = std::map<Price, OrderPointers, std::greater<Price>>;
using Asks = std::map<Price, OrderPointers, std::less<Price>>;
using Orders = std::unordered_map<OrderId, OrderPointer>;

struct Constants {
    static constexpr Price NoPrice = std::numeric_limits<Price>::quiet_NaN();
};