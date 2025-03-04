#define CATCH_CONFIG_MAIN
#include "Order.hpp"
#include "OrderBook.hpp"
#include "Types.hpp"
#include "catch.hpp"

// Define accuracy
#define EPSILON 1e-14

OrderBook orderBook;

// Two generations of an id should not be the same - ids should be unique
TEST_CASE("Orderbook - Unique ID Geneartion", "[tests]") {
    REQUIRE(orderBook.nextOrderId() != Approx(orderBook.nextOrderId()).epsilon(EPSILON));
}

// Test Market Spread Calculations
TEST_CASE("OrderBook - Market Spread Calculation", "[tests]") {
    // Market Spread - No liquidity in the market example
    orderBook.clear();
    REQUIRE(std::isnan(orderBook.getMarketSpread()));

    // Simple Market Spread Example
    orderBook.clear();
    Order buyLimitOrder(Side::BUY, orderBook.nextOrderId(), 1.50, 100);
    Order sellLimitOrder(Side::SELL, orderBook.nextOrderId(), 1.70, 100);
    orderBook.addOrder(buyLimitOrder);
    orderBook.addOrder(sellLimitOrder);
    REQUIRE(orderBook.getMarketSpread() == Approx(0.2).epsilon(EPSILON));

    // Reset orderbook and test again
    orderBook.clear();
    Order buyLimitOrder1(Side::BUY, orderBook.nextOrderId(), 3.50, 31);
    Order sellLimitOrder1(Side::SELL, orderBook.nextOrderId(), 4.92, 34);
    orderBook.addOrder(buyLimitOrder1);
    orderBook.addOrder(sellLimitOrder1);
    REQUIRE(orderBook.getMarketSpread() == Approx(1.42).epsilon(EPSILON));
}

// Test Best Bid/Ask Functions
TEST_CASE("OrderBook - Best Bid/Ask Methods", "[tests]") {
    // Test best bid - also tests correct sorting
    orderBook.clear();

    orderBook.addOrder(Order(Side::BUY, orderBook.nextOrderId(), 1.75, 50));
    REQUIRE(orderBook.getBestBidOrder()->getPrice() == Approx(1.75).epsilon(EPSILON));
    REQUIRE(orderBook.getBestAskOrder() == nullptr);

    orderBook.addOrder(Order(Side::BUY, orderBook.nextOrderId(), 1.85, 50));
    REQUIRE(orderBook.getBestBidOrder()->getPrice() == Approx(1.85).epsilon(EPSILON));
    REQUIRE(orderBook.getBestAskOrder() == nullptr);

    orderBook.addOrder(Order(Side::BUY, orderBook.nextOrderId(), 1.65, 50));
    REQUIRE(orderBook.getBestBidOrder()->getPrice() == Approx(1.85).epsilon(EPSILON));
    REQUIRE(orderBook.getBestAskOrder() == nullptr);

    orderBook.clear();

    orderBook.addOrder(Order(Side::SELL, orderBook.nextOrderId(), 1.65, 50));
    REQUIRE(orderBook.getBestAskOrder()->getPrice() == Approx(1.65).epsilon(EPSILON));
    REQUIRE(orderBook.getBestBidOrder() == nullptr);

    orderBook.addOrder(Order(Side::SELL, orderBook.nextOrderId(), 1.55, 50));
    REQUIRE(orderBook.getBestAskOrder()->getPrice() == Approx(1.55).epsilon(EPSILON));
    REQUIRE(orderBook.getBestBidOrder() == nullptr);

    orderBook.addOrder(Order(Side::SELL, orderBook.nextOrderId(), 1.75, 50));
    REQUIRE(orderBook.getBestAskOrder()->getPrice() == Approx(1.55).epsilon(EPSILON));
    REQUIRE(orderBook.getBestBidOrder() == nullptr);
}

// Test Market Order
TEST_CASE("MarketOrder - Execution Tests", "[tests]") {
    orderBook.clear();

    // Can fully execute the market order to buy with just enough orders
    OrderId limOrderId = orderBook.nextOrderId();
    orderBook.addOrder(Order(Side::SELL, limOrderId, 1.75, 100));
    Order marketOrder1(Side::BUY, orderBook.nextOrderId(), 100);
    orderBook.executeOrder(marketOrder1);
    REQUIRE(orderBook.viewOrder(limOrderId) == nullptr);
}