#include "Order.hpp"
#include "Types.hpp"
#include <iostream>

Order::Order() {}

// General Constructor
Order::Order(OrderType orderType, Side side, OrderId orderId, Price price, Price stopPrice, Quantity initialQuantity,
             IsTriggered isTriggered, IsTriggered wasStopOrder)
    : orderType_(orderType), side_(side), orderId_(orderId), price_(price), stopPrice_(stopPrice), initialQuantity_(initialQuantity),
      isTriggered_(isTriggered), wasStopOrder_(wasStopOrder) {}

// Constructor for Market Order
Order Order::CreateMarketOrder(Side side, OrderId orderId, Quantity initialQuantity) {
    return Order(OrderType::MARKET, side, orderId, Constants::NoPrice, Constants::NoPrice, initialQuantity);
}

// Constructor for Limit Order
Order Order::CreateLimitOrder(Side side, OrderId orderId, Price price, Quantity initialQuantity, IsTriggered isTriggered,
                              bool wasStopOrder) {
    return Order(OrderType::LIMIT, side, orderId, price, Constants::NoPrice, initialQuantity, isTriggered, wasStopOrder);
}

// Constructor for Stop Market Order
Order Order::CreateStopOrder(Side side, OrderId orderId, Price stopPrice, Quantity initialQuantity) {
    return Order(OrderType::STOP_MARKET, side, orderId, Constants::NoPrice, stopPrice, initialQuantity);
}

// Constructor for Stop Limit Order
Order Order::CreateStopLimitOrder(Side side, OrderId orderId, Price stopPrice, Price limitPrice, Quantity initialQuantity) {
    return Order(OrderType::STOP_LIMIT, side, orderId, limitPrice, stopPrice, initialQuantity);
}

// Getter methods
Side Order::getSide() const { return side_; }
OrderId Order::getOrderId() const { return orderId_; }
OrderType Order::getOrderType() const { return orderType_; }
IsTriggered Order::getIsTriggered() const { return isTriggered_; }
IsTriggered Order::getWasStopOrder() const { return wasStopOrder_; }
Price Order::getPrice() const { return price_; }
Price Order::getStopPrice() const { return stopPrice_; }
Quantity Order::getInitialQuantity() const { return initialQuantity_; }
Quantity Order::getRemainingQuantity() const { return initialQuantity_ - filledQuantity_; }
Quantity Order::getFilledQuantity() const { return filledQuantity_; }

// Method that changes
void Order::fill(Quantity quantity) {
    if (quantity > getRemainingQuantity())
        throw std::logic_error(std::format("Order ({}) cannot be filled. Remaining quantity is less than requested quantity.", orderId_));

    // Fill order
    filledQuantity_ += quantity;
}