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

    // Private constructor
    Order(OrderType orderType, Side side, OrderId orderId, Price price, Price stopPrice,
          Quantity initialQuantity);

  public:
    // Constructor for Stop Limit Market Order
    // Order(Side side, OrderId orderId, Price stopPrice, Price limitPrice, Quantity
    // initialQuantity);

    // Different consturctors for each order type
    static Order CreateMarketOrder(Side side, OrderId orderId, Quantity initialQuantity);
    static Order CreateLimitOrder(Side side, OrderId orderId, Price price,
                                  Quantity initialQuantity);
    static Order CreateStopOrder(Side side, OrderId orderId, Price stopPrice,
                                 Quantity initialQuantity);
    static Order CreateStopLimitOrder(Side side, OrderId orderId, Price stopPrice, Price limitPrice,
                                      Quantity initialQuantity);

    OrderType getOrderType();
    Side getSide();
    OrderId getOrderId();
    Price getPrice();
    Price getStopPrice();
    Quantity getInitialQuantity();
    Quantity getRemainingQuantity();
    Quantity getFilledQuantity();
    void fill(Quantity quantity);
};
