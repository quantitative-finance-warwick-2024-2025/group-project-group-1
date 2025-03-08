#pragma once
#include "Types.hpp"

class Order {
  protected:
    OrderType orderType_;
    Side side_;
    OrderId orderId_;
    Price price_;
    Price stopPrice_;
    Quantity initialQuantity_;
    Quantity filledQuantity_{0};

  public:
    // Constructors
    Order(OrderType orderType, Side side, OrderId orderId, Price price, Price stopPrice,
          Quantity initialQuantity);

    // Constructor for Market Order
    Order(Side side, OrderId orderId, Quantity initialQuantity);

    // Constructor for Limit Order
    Order(Side side, OrderId orderId, Price price, Quantity initialQuantity);

    // constructor for Stop Market/limit orders
    Order(Side side, OrderId orderId, Price stopPrice, Price limitPrice, 
          Quantity initialQuantity, OrderType orderType);

    OrderType getOrderType();
    Side getSide();
    OrderId getOrderId();
    Price getPrice();
    Price getStopPrice() const;
    Quantity getInitialQuantity();
    Quantity getRemainingQuantity();
    Quantity getFilledQuantity();
    void fill(Quantity quantity);
};
