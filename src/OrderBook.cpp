#include "OrderBook.hpp"
#include "Order.hpp"
#include "Types.hpp"
#include <iomanip>
#include <iostream>

// Template method that handles filling a market order
template <typename Compare>
void fillMarketOrder(std::map<Price, OrderPointers, Compare> &book, Quantity &remainingQuantity,
                     OrderPointers &ordersFilled) {
    for (const auto &[price, ordersList] : book) {
        if (remainingQuantity == 0)
            break;

        for (auto &orderPointer : ordersList) {
            if (remainingQuantity == 0)
                break;
            Quantity available = orderPointer->getRemainingQuantity();
            if (available > remainingQuantity) {
                orderPointer->fill(remainingQuantity);
                remainingQuantity = 0;
            } else {
                ordersFilled.push_back(orderPointer);
                remainingQuantity -= available;
                orderPointer->fill(available);
            }
        }
    }
}

// Method that handles submission of orders to the orderbook
void OrderBook::submitOrder(Order order) {
    // Get order details
    OrderType orderType = order.getOrderType();
    Side orderSide = order.getSide();
    Quantity remainingQuantity = order.getRemainingQuantity();
    OrderPointers ordersFilled{};
    OrderId orderId = order.getOrderId();
    OrderPointer orderPointer = std::make_shared<Order>(order);
    Price limitPrice = order.getPrice();

    // If its a market order, handle execution
    if (orderType == OrderType::MARKET) {
        if (orderSide == Side::BUY) {
            fillMarketOrder(asks_, remainingQuantity, ordersFilled);
        } else {
            fillMarketOrder(bids_, remainingQuantity, ordersFilled);
        }

        for (OrderPointer currentOrderPointer : ordersFilled) {
            removeOrder(currentOrderPointer->getOrderId());
        }

        if (remainingQuantity == 0) {
            std::cout << "   > Fully executed market order." << std::endl;
        } else {
            std::cout << "   > Partially executed market order. Remaining quantity: "
                      << remainingQuantity << std::endl;
        }
    }

    // If its a limit order, handle adding to the book
    if (orderType == OrderType::LIMIT) {
        // Add the order to the orders
        if (orders_.contains(orderId)) {
            throw std::logic_error(
                std::format("Order ({}) already exists. Duplication occurred.", orderId));
        }
        orders_[orderId] = orderPointer;
        // Add the order to the book
        if (orderSide == Side::BUY) {
            // Add order to asks
            bids_[limitPrice].push_back(orderPointer);
        } else {
            // Add orders to bids
            asks_[limitPrice].push_back(orderPointer);
        }

        std::cout << "   > Added Limit (" << (orderPointer->getSide() == Side::BUY ? "buy" : "sell")
                  << ") order (" << orderId << ") for " << orderPointer->getRemainingQuantity()
                  << " @ " << orderPointer->getPrice() << std::endl;

        matchOrders();
    }
}

// Method that prints the orders of the order book
void OrderBook::printOrders() {
    std::cout << "--------- Registered Orders ---------" << std::endl;
    if (orders_.size() > 0) {

        for (std::pair<OrderId, OrderPointer> ordersList : orders_) {
            OrderId orderId = ordersList.first;
            OrderPointer orderPointer = ordersList.second;
            std::cout << "   > Limit (" << (orderPointer->getSide() == Side::BUY ? "buy" : "sell")
                      << ") order (" << orderId << ") for " << orderPointer->getRemainingQuantity()
                      << " @ " << orderPointer->getPrice() << std::endl;
        }
    } else {
        std::cout << "   > No orders found." << std::endl;
    }
}

// Method that prints the ask side of the order book
void OrderBook::printAsks() {
    std::cout << "------------- Asks Side -------------" << std::endl;
    if (asks_.size() > 0) {
        for (std::pair<Price, OrderPointers> ordersList : asks_) {
            Price levelPrice = ordersList.first;
            OrderPointers orderPointers = ordersList.second;

            std::cout << "Ask Level = " << levelPrice << std::endl;
            for (OrderPointer orderPointer : orderPointers) {
                std::cout << "   > Limit ("
                          << (orderPointer->getSide() == Side::BUY ? "buy" : "sell") << ") order ("
                          << orderPointer->getOrderId() << ") for "
                          << orderPointer->getRemainingQuantity() << " @ "
                          << orderPointer->getPrice() << std::endl;
            }
        }
    } else {
        std::cout << "   > No asks found." << std::endl;
    }
}

// Method that prints the bid side of the order book
void OrderBook::printBids() {
    std::cout << "------------- Bids Side -------------" << std::endl;
    if (bids_.size() > 0) {
        for (std::pair<Price, OrderPointers> ordersList : bids_) {
            Price levelPrice = ordersList.first;
            OrderPointers orderPointers = ordersList.second;

            std::cout << "Bids Level = " << levelPrice << std::endl;
            for (OrderPointer orderPointer : orderPointers) {
                std::cout << "   > Limit ("
                          << (orderPointer->getSide() == Side::BUY ? "buy" : "sell") << ") order ("
                          << orderPointer->getOrderId() << ") for "
                          << orderPointer->getRemainingQuantity() << " @ "
                          << orderPointer->getPrice() << std::endl;
            }
        }
    } else {
        std::cout << "   > No bids found." << std::endl;
    }
}

// Method that gets the order from the book
OrderPointer OrderBook::viewOrder(OrderId orderId) {
    // Check if order exists
    if (!orders_.contains(orderId)) {
        return nullptr;
    }

    // Return order
    OrderPointer orderPointer = orders_.at(orderId);
    // std::cout << std::format("   > Order ({}) Found. Level price is ", orderId)
    //           << orderPointer->getPrice() << std::endl;
    return orderPointer;
}

// Method that removes order from the book
void OrderBook::removeOrder(OrderId orderId) {
    // Check if order exists
    if (!orders_.contains(orderId)) {
        std::cout << "   > Order ID doesn't exist. \n";
        return;
    }

    // Get the order
    OrderPointer orderPointer = viewOrder(orderId);
    Price levelPrice = orderPointer->getPrice();

    // Remove the order from list of price level, or delete whole price level
    orders_.erase(orderId);
    if (orderPointer->getSide() == Side::BUY) {
        OrderPointers orderPointers = bids_[levelPrice];
        if (orderPointers.size() > 1) {
            bids_[levelPrice].remove(orderPointer);
        } else {
            bids_.erase(levelPrice);
        }
    } else {
        OrderPointers orderPointers = asks_[levelPrice];
        if (orderPointers.size() > 1) {
            asks_[levelPrice].remove(orderPointer);
        } else {
            asks_.erase(levelPrice);
        }
    }

    std::cout << std::format("   > Removed Order ({}) from the book.", orderId) << std::endl;
}

// Method to execute a marker order
void OrderBook::executeOrder(Order order) {
    // Check that this is a market order
    if (order.getOrderType() != OrderType::MARKET) {
        throw std::logic_error(
            std::format("Order ({}) is not a market order.", order.getOrderId()));
    }

    Side side = order.getSide();
    Quantity remainingQuantity = order.getRemainingQuantity();
    OrderPointers ordersFilled{};

    std::cout << "Market Execution Submitted" << std::endl;
    std::cout << "   > Filling " << (side == Side::BUY ? "buy" : "sell") << "  market order for "
              << remainingQuantity << std::endl;

    if (side == Side::BUY) {
        // Check ask side
        if (asks_.size() == 0) {
            std::cout << "   > No possible trades found." << std::endl;
            return;
        }
        fillMarketOrder(asks_, remainingQuantity, ordersFilled);
    } else {
        // Check bid size
        if (bids_.size() == 0) {
            std::cout << "   > No possible trades found." << std::endl;
            return;
        }
        fillMarketOrder(bids_, remainingQuantity, ordersFilled);
    }

    for (OrderPointer currentOrderPointer : ordersFilled) {
        removeOrder(currentOrderPointer->getOrderId());
    }

    if (remainingQuantity == 0) {
        std::cout << "   > Fully executed market order." << std::endl;
    } else {
        std::cout << "   > Partially executed market order. Remaining quantity "
                  << remainingQuantity << std::endl;
    }
}

void OrderBook::getBookSnapshot() {
    // Define column widths
    std::cout << "" << std::endl;
    const int priceWidth = 10;
    const int qtyWidth = 8;

    // Print header
    std::cout << std::setw(priceWidth) << "Asks" << std::setw(qtyWidth) << "Qty"
              << "  |  " << std::setw(priceWidth) << "Bids" << std::setw(qtyWidth) << "Qty" << '\n';

    std::cout << "------------------------------------------\n";

    // Iterators for bids and asks
    auto bidIt = bids_.begin();
    auto askIt = asks_.begin();

    while (bidIt != bids_.end() || askIt != asks_.end()) {
        // Print Ask side (Sell Orders)
        if (askIt != asks_.end()) {
            double askPrice = askIt->first;
            int askQty = 0;
            for (const auto &order : askIt->second)
                askQty += order->getRemainingQuantity(); // Sum total quantity at this price level
            std::cout << std::setw(priceWidth) << askPrice << std::setw(qtyWidth) << askQty;
            ++askIt;
        } else {
            // Print empty spaces if there are no more asks
            std::cout << std::setw(priceWidth) << " " << std::setw(qtyWidth) << " ";
        }

        std::cout << "  |  ";

        // Print Bid side (Buy Orders)
        if (bidIt != bids_.end()) {
            double bidPrice = bidIt->first;
            int bidQty = 0;
            for (const auto &order : bidIt->second)
                bidQty += order->getRemainingQuantity(); // Sum total quantity at this price level
            std::cout << std::setw(priceWidth) << bidPrice << std::setw(qtyWidth) << bidQty;
            ++bidIt;
        } else {
            // Print empty spaces if there are no more bids
            std::cout << std::setw(priceWidth) << " " << std::setw(qtyWidth) << " ";
        }

        std::cout << '\n'; // Move to the next row
    }

    if (bids_.size() == 0 && asks_.size() == 0) {
        std::cout << "     EMPTY BOOK" << std::endl;
    }

    std::cout << "------------------------------------------\n";

    std::cout << "" << std::endl;
}

// Method that generates an order id
OrderId OrderBook::nextOrderId() {
    currentOrderId_ += 1;
    return currentOrderId_;
}

void OrderBook::matchOrders() {
    std::cout << "   > Matching algorithm triggered." << std::endl;

    // Continue matching as long as both sides of the book have orders
    while (!bids_.empty() && !asks_.empty()) {
        // Identify best bid and best ask
        Bids::iterator bestBidIt = bids_.begin();
        Asks::iterator bestAskIt = asks_.begin();
        Price bestBidPrice = bestBidIt->first;
        Price bestAskPrice = bestAskIt->first;

        // If no crossing, exit
        if (bestBidPrice < bestAskPrice)
            break;

        // Get the orders at these best price levels
        OrderPointers &bidOrdersAtLevel = bestBidIt->second;
        OrderPointers &askOrdersAtLevel = bestAskIt->second;

        bool matchedAnything = false;
        auto bidIt = bidOrdersAtLevel.begin();

        // Iterate through the bid orders at this price level
        while (bidIt != bidOrdersAtLevel.end() && !askOrdersAtLevel.empty()) {
            OrderPointer bidOrder = *bidIt;
            OrderPointer askOrder = askOrdersAtLevel.front();

            // Determine the trade quantity as the minimum remaining quantity of the two orders
            Quantity tradeQty =
                std::min(bidOrder->getRemainingQuantity(), askOrder->getRemainingQuantity());
            Price tradePrice = bestAskPrice;

            // Fill both orders
            bidOrder->fill(tradeQty);
            askOrder->fill(tradeQty);

            std::cout << "   > Trade executed: " << tradeQty << " @ " << tradePrice
                      << " between BUY order " << bidOrder->getOrderId() << " and SELL order "
                      << askOrder->getOrderId() << std::endl;

            matchedAnything = true;

            // If the ask order is fully filled, remove it from the book.
            if (askOrder->getRemainingQuantity() == 0) {
                removeOrder(askOrder->getOrderId());
            }

            // If the bid order is fully filled, remove it and break out of the inner loop
            if (bidOrder->getRemainingQuantity() == 0) {
                removeOrder(bidOrder->getOrderId());
                break;
            } else {
                // If not fully filled, continue to the next bid order.
                ++bidIt;
            }
        }

        // If nothing was matched, break
        if (!matchedAnything) {
            break;
        }
    }
}

// Method that returns the best ask order
OrderPointer OrderBook::getBestAskOrder() {
    if (asks_.empty()) {
        return nullptr;
    }
    OrderPointer bestAskOrderPtr = asks_.begin()->second.front();
    return bestAskOrderPtr;
}

// Method that returns the best bid order
OrderPointer OrderBook::getBestBidOrder() {
    if (bids_.empty()) {
        return nullptr;
    }
    OrderPointer bestBidOrderPtr = bids_.begin()->second.front();
    return bestBidOrderPtr;
}

// Methods that returns the market spread
Price OrderBook::getMarketSpread() {
    // Get best bid and ask
    OrderPointer bestAskOrderPtr = getBestAskOrder();
    OrderPointer bestBidOrderPtr = getBestBidOrder();
    // Check if we have nullptr
    if (bestAskOrderPtr == nullptr || bestBidOrderPtr == nullptr) {
        return Constants::NoPrice;
    }
    Price marketSpread = bestAskOrderPtr->getPrice() - bestBidOrderPtr->getPrice();
    return marketSpread;
}

// Method that gets the available asks quantity at a level
Quantity OrderBook::getAskQuantityAtLevel(Price priceLevel) {
    // Check if any orders exist
    Asks::iterator asksIterator = asks_.find(priceLevel);
    if (asksIterator == asks_.end())
        return 0;

    // Sum the remaining quantity
    Quantity availableQuantity{0};
    for (OrderPointer orderPointer : asksIterator->second) {
        availableQuantity += orderPointer->getRemainingQuantity();
    }

    return availableQuantity;
}

// Method that gets the available bids quantity at a level
Quantity OrderBook::getBidQuantityAtLevel(Price priceLevel) {
    // Check if any orders exist
    Bids::iterator bidsIterator = bids_.find(priceLevel);
    if (bidsIterator == bids_.end())
        return 0;

    Quantity availableQuantity{0};
    for (OrderPointer orderPointer : bidsIterator->second) {
        availableQuantity += orderPointer->getRemainingQuantity();
    }

    return availableQuantity;
}

// Method to reset the orderbook
void OrderBook::clear() {
    bids_.clear();
    asks_.clear();
    orders_.clear();
    currentOrderId_ = 0;
}
