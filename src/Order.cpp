#include "Order.hpp"
#include "Types.hpp"
#include <iostream>

// General Constructor
Order::Order(OrderType orderType, Side side, OrderId orderId, Price price, Price stopPrice,
             Quantity initialQuantity)
    : orderType_(orderType), side_(side), orderId_(orderId), price_(price), stopPrice_(stopPrice),
      initialQuantity_(initialQuantity) {}

// Constructor for Market Order
Order Order::CreateMarketOrder(Side side, OrderId orderId, Quantity initialQuantity) {
    return Order(OrderType::MARKET, side, orderId, Constants::NoPrice, Constants::NoPrice,
                 initialQuantity);
}

// Constructor for Limit Order
Order Order::CreateLimitOrder(Side side, OrderId orderId, Price price, Quantity initialQuantity) {
    return Order(OrderType::LIMIT, side, orderId, price, Constants::NoPrice, initialQuantity);
}

OrderType Order::getOrderType() { return orderType_; }
OrderId Order::getOrderId() { return orderId_; }
Side Order::getSide() { return side_; }
Price Order::getPrice() { return price_; }
Quantity Order::getInitialQuantity() { return initialQuantity_; }
Quantity Order::getRemainingQuantity() { return initialQuantity_ - filledQuantity_; }
Quantity Order::getFilledQuantity() { return filledQuantity_; }

void Order::fill(Quantity quantity) {
    if (quantity > getRemainingQuantity())
        throw std::logic_error(std::format(
            "Order ({}) cannot be filled. Remaining quantity is less than requested quantity.",
            orderId_));

    // Fill order
    filledQuantity_ += quantity;
}