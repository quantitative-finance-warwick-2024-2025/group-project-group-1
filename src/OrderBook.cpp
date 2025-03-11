#include "OrderBook.hpp"
#include "Order.hpp"
#include "Types.hpp"
#include <iomanip>
#include <iostream>

// Template method that handles filling a market order
template <typename Compare>
void fillMarketOrder(std::map<Price, OrderPointers, Compare> &book, Quantity &remainingQuantity, OrderPointers &ordersFilled) {
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

///////////////////////////////////////////////////////////////
// Methods
///////////////////////////////////////////////////////////////

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

    // Order IDs must be unique
    if (orders_.contains(orderId)) {
        throw std::logic_error("Order (" + std::to_string(orderId) + ") already exists. Duplication occurred.");
    }

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
            std::cout << YELLOW << "[System] Market order (#" << orderId << ") for " << order.getInitialQuantity()
                      << " units executed in full." << RESET << std::endl;
        } else {
            std::cout << YELLOW << "[System] Market order (#" << orderId
                      << ") partially executed. Remaining quantity: " << remainingQuantity << " units." << RESET << std::endl;
        }

        matchOrders();
        checkStopOrders();

        return;
    }

    // If its a limit order, handle adding to the book
    if (orderType == OrderType::LIMIT) {
        // Add the order to the orders
        orders_[orderId] = orderPointer;
        // Add the order to the book
        if (orderSide == Side::BUY) {
            // Add order to bids
            bids_[limitPrice].push_back(orderPointer);
        } else {
            // Add orders to asks
            asks_[limitPrice].push_back(orderPointer);
        }

        std::cout << GREEN << "[System] Limit (" << (orderPointer->getSide() == Side::BUY ? "buy" : "sell") << ") order (#" << orderId
                  << ") for " << orderPointer->getRemainingQuantity() << " units @ " << orderPointer->getPrice()
                  << " has been added to the order book." << RESET << std::endl;

        matchOrders();
        checkStopOrders();

        return;
    }

    // If its a stop order
    if (orderType == OrderType::STOP_LIMIT || orderType == OrderType::STOP_MARKET) {
        // Add the order to the orders
        orders_[orderId] = orderPointer;
        if (orderSide == Side::BUY) {
            stopBids_[order.getStopPrice()].push_back(orderPointer);
        } else {
            stopAsks_[order.getStopPrice()].push_back(orderPointer);
        }

        if (orderType == OrderType::STOP_MARKET) {
            std::cout << GREEN << "[System] Stop-Market (" << (orderSide == Side::BUY ? "buy" : "sell") << ") order (#"
                      << order.getOrderId() << ") for " << order.getRemainingQuantity() << " units with stop price " << order.getStopPrice()
                      << " has been added." << RESET << std::endl;
        } else {
            std::cout << GREEN << "[System] Stop-Limit (" << (orderSide == Side::BUY ? "buy" : "sell") << ") order (#" << order.getOrderId()
                      << ") for " << order.getRemainingQuantity() << " units with stop price " << order.getStopPrice()
                      << " and limit price " << order.getPrice() << " has been added." << RESET << std::endl;
        }

        checkStopOrders();
    }
}

// Method that prints the orders of the order book
void OrderBook::printOrders() {
    std::cout << "--------- Registered Orders ---------" << std::endl;
    if (orders_.size() > 0) {

        for (std::pair<OrderId, OrderPointer> ordersList : orders_) {
            OrderId orderId = ordersList.first;
            OrderPointer orderPointer = ordersList.second;
            std::cout << "   > Limit (" << (orderPointer->getSide() == Side::BUY ? "buy" : "sell") << ") order (" << orderId << ") for "
                      << orderPointer->getRemainingQuantity() << " @ " << orderPointer->getPrice() << std::endl;
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
                std::cout << "   > Limit (" << (orderPointer->getSide() == Side::BUY ? "buy" : "sell") << ") order ("
                          << orderPointer->getOrderId() << ") for " << orderPointer->getRemainingQuantity() << " @ "
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
                std::cout << "   > Limit (" << (orderPointer->getSide() == Side::BUY ? "buy" : "sell") << ") order ("
                          << orderPointer->getOrderId() << ") for " << orderPointer->getRemainingQuantity() << " @ "
                          << orderPointer->getPrice() << std::endl;
            }
        }
    } else {
        std::cout << "   > No bids found." << std::endl;
    }
}

// Method that removes order from the book
void OrderBook::removeOrder(OrderId orderId, bool print) {
    // Check if order exists
    if (!orders_.contains(orderId)) {
        if (print) {
            std::cout << RED << "[System] Order " << orderId << " doesn't exist." << RESET << std::endl;
        }
        return;
    }

    // Get the order
    OrderPointer orderPointer = getOrder(orderId);

    // Get orderType
    OrderType orderType = orderPointer->getOrderType();
    Price levelPrice = orderPointer->getPrice();

    if (orderType == OrderType::STOP_LIMIT || orderType == OrderType::STOP_MARKET) {
        removeStopOrder(orderPointer, print);
        return;
    }

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

    if (print) {
        std::cout << GREEN << "[System] Removed order (" << orderId << ") from the book." << RESET << std::endl;
    }
}

// Method that gets the current book snapshot
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

// Method that matches the limit order book
void OrderBook::matchOrders() {
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
            Quantity tradeQty = std::min(bidOrder->getRemainingQuantity(), askOrder->getRemainingQuantity());
            Price tradePrice = bestAskPrice;

            // Fill both orders
            bidOrder->fill(tradeQty);
            askOrder->fill(tradeQty);

            std::cout << YELLOW << "[System] Trade executed: " << tradeQty << " units @ " << tradePrice << " between BUY order (#"
                      << bidOrder->getOrderId() << ") and SELL order (#" << askOrder->getOrderId() << ")." << RESET << std::endl;

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

// Method that checks stop orders
void OrderBook::checkStopOrders() {
    // Calculate the current market price from active orders.
    Price marketPrice = getMarketPrice();
    if (std::isnan(marketPrice))
        return; // No valid market price available.

    // Handle stops
    if (!stopBids_.empty()) {
        StopBids::iterator bidIt = stopBids_.begin(); // first stop price level
        Price bidStopPrice = bidIt->first;            // lowest stop price

        // Trigger condition: marketPrice >= stopPrice
        if (marketPrice >= bidStopPrice) {
            // Get the first stop order at this price level.
            OrderPointer orderPtr = bidIt->second.front();
            Order triggeredOrder;

            if (orderPtr->getOrderType() == OrderType::STOP_MARKET) {
                triggeredOrder = Order::CreateMarketOrder(orderPtr->getSide(), orderPtr->getOrderId(), orderPtr->getRemainingQuantity());
            } else { // STOP_LIMIT order
                triggeredOrder = Order::CreateLimitOrder(orderPtr->getSide(), orderPtr->getOrderId(), orderPtr->getPrice(),
                                                         orderPtr->getRemainingQuantity(), true, true);
            }

            std::cout << YELLOW << "[System] Triggered BUY stop order (#" << orderPtr->getOrderId() << ") at market price " << marketPrice
                      << RESET << std::endl; // Remove the stop order
            removeStopOrder(orderPtr);
            // Re-submit the converted order
            submitOrder(triggeredOrder);
            matchOrders();
            return;
        }
    }

    // handle asks
    if (!stopAsks_.empty()) {
        auto askIt = stopAsks_.begin();    // first stop price level
        Price askStopPrice = askIt->first; // highest stop price

        // Trigger condition: marketPrice <= stopPrice
        if (marketPrice <= askStopPrice) {
            // Get first stop order at this price level
            OrderPointer orderPtr = askIt->second.front();
            Order triggeredOrder;

            if (orderPtr->getOrderType() == OrderType::STOP_MARKET) {
                triggeredOrder = Order::CreateMarketOrder(orderPtr->getSide(), orderPtr->getOrderId(), orderPtr->getRemainingQuantity());
            } else { // STOP_LIMIT order
                triggeredOrder = Order::CreateLimitOrder(orderPtr->getSide(), orderPtr->getOrderId(), orderPtr->getPrice(),
                                                         orderPtr->getRemainingQuantity(), true, true);
            }

            // Remove stop order
            std::cout << YELLOW << "[SYSTEM] Triggered SELL stop order (#" << orderPtr->getOrderId() << ") at market price " << marketPrice
                      << RESET << std::endl;
            removeStopOrder(orderPtr);
            // Re-submit converted order
            submitOrder(triggeredOrder);
            matchOrders();
            return;
        }
    }
}

// Method to reset the orderbook
void OrderBook::clear() {
    bids_.clear();
    asks_.clear();
    orders_.clear();
    currentOrderId_ = 0;
}

// Remove stop order from the orderbook
void OrderBook::removeStopOrder(OrderPointer orderPointer, bool print) {
    // Check if the order exists in the main orders map.
    OrderId orderId = orderPointer->getOrderId();
    if (!orders_.contains(orderId)) {
        if (print) {
            std::cout << RED << "[System] Order ID doesn't exist." << RESET << std::endl;
        }
        return;
    }

    // Remove the order from the main orders map.
    orders_.erase(orderId);

    // Use the stop price for removal from the stop containers.
    Price stopPrice = orderPointer->getStopPrice();

    // Remove from the appropriate stop container.
    if (orderPointer->getSide() == Side::BUY) {
        // For BUY stop orders, remove from stopBids_.
        auto it = stopBids_.find(stopPrice);
        if (it != stopBids_.end()) {
            if (it->second.size() > 1) {
                it->second.remove(orderPointer);
            } else {
                stopBids_.erase(it);
            }
        }
    } else {
        // For SELL stop orders, remove from stopAsks_.
        auto it = stopAsks_.find(stopPrice);
        if (it != stopAsks_.end()) {
            if (it->second.size() > 1) {
                it->second.remove(orderPointer);
            } else {
                stopAsks_.erase(it);
            }
        }
    }

    if (print) {
        std::cout << RED << "[System] Removed stop order (" << orderId << ") from the book." << RESET << std::endl;
    }
}

///////////////////////////////////////////////////////////////
// Getter Methods
///////////////////////////////////////////////////////////////

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

// Method that returns the best ask order
OrderPointer OrderBook::getBestAskOrder() const {
    if (asks_.empty()) {
        return nullptr;
    }
    OrderPointer bestAskOrderPtr = asks_.begin()->second.front();
    return bestAskOrderPtr;
}

// Method that returns the best bid order
OrderPointer OrderBook::getBestBidOrder() const {
    if (bids_.empty()) {
        return nullptr;
    }
    OrderPointer bestBidOrderPtr = bids_.begin()->second.front();
    return bestBidOrderPtr;
}

// Method that gets the order from the book
OrderPointer OrderBook::getOrder(OrderId orderId) {
    // Check if order exists
    if (!orders_.contains(orderId)) {
        return nullptr;
    }

    // Return order
    OrderPointer orderPointer = orders_.at(orderId);

    Side orderSide = orderPointer->getSide();
    Price limitPrice = orderPointer->getPrice();
    Price stopPrice = orderPointer->getStopPrice();
    OrderType orderType = orderPointer->getOrderType();

    if (orderType == OrderType::LIMIT) {
        // Check if it is present in the actual order book (bids or asks)
        if (orderSide == Side::BUY) {
            if (bids_.contains(limitPrice)) {
                for (OrderPointer bidOrder : bids_.at(limitPrice)) {
                    if (bidOrder->getOrderId() == orderId) {
                        return orderPointer; // Order is in the bids
                    }
                }
            }
        } else { // Side::SELL
            if (asks_.contains(limitPrice)) {
                for (OrderPointer askOrder : asks_.at(limitPrice)) {
                    if (askOrder->getOrderId() == orderId) {
                        return orderPointer; // Order is in the asks
                    }
                }
            }
        }
    }

    if (orderType == OrderType::STOP_LIMIT || orderType == OrderType::STOP_MARKET) {
        // Check if it is present in the actual order book (bids or asks)
        if (orderSide == Side::BUY) {
            if (stopBids_.contains(stopPrice)) {
                for (OrderPointer bidOrder : stopBids_.at(stopPrice)) {
                    if (bidOrder->getOrderId() == orderId) {
                        return orderPointer; // Order is in the bids
                    }
                }
            }
        } else { // Side::SELL
            if (stopAsks_.contains(stopPrice)) {
                for (OrderPointer askOrder : stopAsks_.at(stopPrice)) {
                    if (askOrder->getOrderId() == orderId) {
                        return orderPointer; // Order is in the asks
                    }
                }
            }
        }
    }

    // If the order is not found in bids or asks, return nullptr
    return nullptr;
}

// Methods that returns the market spread
Price OrderBook::getMarketSpread() const {
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

// Method that returns the market price
Price OrderBook::getMarketPrice() const {
    OrderPointer bestAsk = getBestAskOrder();
    OrderPointer bestBid = getBestBidOrder();

    // Return the midpoint of best bid and best ask if both are available, otherwise return best bid or best ask
    if (bestAsk != nullptr && bestBid != nullptr) {
        return (bestAsk->getPrice() + bestBid->getPrice()) / 2.0;
    } else if (bestAsk != nullptr) {
        return bestAsk->getPrice();
    } else if (bestBid != nullptr) {
        return bestBid->getPrice();
    }

    return Constants::NoPrice; // No valid price available - no liquidity in the market
}
