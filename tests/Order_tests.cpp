#define CATCH_CONFIG_MAIN
#include "Order.hpp"
#include "Types.hpp"
#include "catch.hpp"

// Test core order types first
TEST_CASE("Order - Market Order Construction", "[order]") {
    OrderId orderId = 1;
    Quantity qty = 100;
    Order order(Side::BUY, orderId, qty);
    
    SECTION("Verify Order Type") {
        REQUIRE(order.getOrderType() == OrderType::MARKET);
    }
    
    SECTION("Verify Price") {
        REQUIRE(std::isnan(order.getPrice()));
    }
    
    SECTION("Verify Quantities") {
        REQUIRE(order.getInitialQuantity() == qty);
        REQUIRE(order.getRemainingQuantity() == qty);
        REQUIRE(order.getFilledQuantity() == 0);
    }
}

TEST_CASE("Order - Limit Order Construction", "[order]") {
    OrderId orderId = 2;
    Quantity qty = 50;
    Price price = 100.50;
    Order order(Side::SELL, orderId, price, qty);
    
    SECTION("Verify Order Type") {
        REQUIRE(order.getOrderType() == OrderType::LIMIT);
    }
    
    SECTION("Verify Price") {
        REQUIRE(order.getPrice() == Approx(price).epsilon(1e-9));
    }
    
    SECTION("Verify Quantities") {
        REQUIRE(order.getInitialQuantity() == qty);
        REQUIRE(order.getRemainingQuantity() == qty);
        REQUIRE(order.getFilledQuantity() == 0);
    }
}

TEST_CASE("Order - Stop Market Order Construction", "[order]") {
    OrderId orderId = 3;
    Quantity qty = 200;
    Price stopPrice = 150.0;
    Order order(Side::BUY, orderId, stopPrice, Constants::NoPrice, qty, OrderType::STOP_MARKET);

    SECTION("Verify Order Type") {
        REQUIRE(order.getOrderType() == OrderType::STOP_MARKET);
    }
    
    SECTION("Verify Stop Price") {
        REQUIRE(order.getStopPrice() == Approx(stopPrice).epsilon(1e-9));
    }
    
    SECTION("Verify Limit Price (Should be NaN)") {
        REQUIRE(std::isnan(order.getPrice()));
    }
}

TEST_CASE("Order - Stop Limit Order Construction", "[order]") {
    OrderId orderId = 4;
    Quantity qty = 75;
    Price stopPrice = 95.0;
    Price limitPrice = 94.5;
    Order order(Side::SELL, orderId, stopPrice, limitPrice, qty, OrderType::STOP_LIMIT);

    SECTION("Verify Order Type") {
        REQUIRE(order.getOrderType() == OrderType::STOP_LIMIT);
    }
    
    SECTION("Verify Prices") {
        REQUIRE(order.getStopPrice() == Approx(stopPrice).epsilon(1e-9));
        REQUIRE(order.getPrice() == Approx(limitPrice).epsilon(1e-9));
    }
}

TEST_CASE("Order - Fill Method", "[order]") {
    OrderId orderId = 5;
    Quantity initialQty = 200;
    Order order(Side::BUY, orderId, 150.75, initialQty);
    
    SECTION("Partial Fill") {
        order.fill(50);
        REQUIRE(order.getFilledQuantity() == 50);
        REQUIRE(order.getRemainingQuantity() == initialQty - 50);
    }
    
    SECTION("Full Fill") {
        order.fill(initialQty);
        REQUIRE(order.getFilledQuantity() == initialQty);
        REQUIRE(order.getRemainingQuantity() == 0);
    }
    
    SECTION("Overfill Throws Exception") {
        REQUIRE_THROWS_AS(order.fill(initialQty + 1), std::logic_error);
    }
}


TEST_CASE("Order - Getters Validation", "[order]") {
    OrderId orderId = 6;
    Side side = Side::SELL;
    Price price = 75.25;
    Quantity qty = 300;
    Price stopPrice = 80.0;
    
    Order limitOrder(side, orderId, price, qty);
    Order marketOrder(side, orderId, qty);
    Order stopMarketOrder(side, orderId, stopPrice, Constants::NoPrice, qty, OrderType::STOP_MARKET);
    Order stopLimitOrder(side, orderId, stopPrice, price, qty, OrderType::STOP_LIMIT);
    
    SECTION("Order ID Consistency") {
        REQUIRE(limitOrder.getOrderId() == orderId);
        REQUIRE(stopLimitOrder.getOrderId() == orderId);
    }
    
    SECTION("Side Validation") {
        REQUIRE(marketOrder.getSide() == side);
        REQUIRE(stopMarketOrder.getSide() == side);
    }
    
    SECTION("Price Getters") {
        REQUIRE(limitOrder.getPrice() == Approx(price).epsilon(1e-9));
        REQUIRE(std::isnan(marketOrder.getPrice()));
        REQUIRE(stopLimitOrder.getPrice() == Approx(price).epsilon(1e-9));
    }
    
    SECTION("Stop Price Getter") {
        REQUIRE(stopMarketOrder.getStopPrice() == Approx(stopPrice).epsilon(1e-9));
        REQUIRE(stopLimitOrder.getStopPrice() == Approx(stopPrice).epsilon(1e-9));
    }
}