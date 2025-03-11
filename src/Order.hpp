#pragma once
#include "Types.hpp"

class Order {
  private:
    // Variables
    Side side_;
    OrderType orderType_;
    OrderId orderId_;
    IsTriggered isTriggered_{false};
    IsTriggered wasStopOrder_{false};
    Price price_;
    Price stopPrice_;
    Quantity initialQuantity_;
    Quantity filledQuantity_{0};

    // Private constructor
    Order(OrderType orderType, Side side, OrderId orderId, Price price, Price stopPrice, Quantity initialQuantity,
          IsTriggered isTriggered = false, bool wasStopOrder = false);

  public:
    Order();
    // Different consturctors for each order type
    static Order CreateMarketOrder(Side side, OrderId orderId, Quantity initialQuantity);
    static Order CreateLimitOrder(Side side, OrderId orderId, Price price, Quantity initialQuantity, IsTriggered isTriggered = false,
                                  bool wasStopOrder = false);
    static Order CreateStopOrder(Side side, OrderId orderId, Price stopPrice, Quantity initialQuantity);
    static Order CreateStopLimitOrder(Side side, OrderId orderId, Price stopPrice, Price limitPrice, Quantity initialQuantity);

    // Getters
    Side getSide() const;
    OrderId getOrderId() const;
    OrderType getOrderType() const;
    IsTriggered getIsTriggered() const;
    IsTriggered getWasStopOrder() const;
    Price getPrice() const;
    Price getStopPrice() const;
    Quantity getInitialQuantity() const;
    Quantity getRemainingQuantity() const;
    Quantity getFilledQuantity() const;

    // Methods
    void fill(Quantity quantity);
};
