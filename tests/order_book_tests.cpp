#define CATCH_CONFIG_MAIN
#include "Order.hpp"
#include "OrderBook.hpp"
#include "Types.hpp"
#include "catch.hpp"

// Define accuracy
#define EPSILON 1e-14

OrderBook orderBook;

// Test 1: Unique ID Generation
TEST_CASE("OrderBook - Unique ID Generation", "[tests]") {
    orderBook.clear();
    REQUIRE(orderBook.nextOrderId() != Approx(orderBook.nextOrderId()).epsilon(EPSILON));
}

// Test 2: Market Spread Calculations
TEST_CASE("OrderBook - Market Spread Calculation", "[tests]") {
    orderBook.clear();

    // No liquidity - market just initialised
    REQUIRE(std::isnan(orderBook.getMarketSpread()));

    // Simple market spread example
    orderBook.clear();
    Order buyLimitOrder = Order::CreateLimitOrder(Side::BUY, orderBook.nextOrderId(), 1.50, 100);
    Order sellLimitOrder = Order::CreateLimitOrder(Side::SELL, orderBook.nextOrderId(), 1.70, 100);
    orderBook.submitOrder(buyLimitOrder);
    orderBook.submitOrder(sellLimitOrder);

    REQUIRE(orderBook.getMarketSpread() == Approx(0.2).epsilon(EPSILON));

    // Another market spread example
    orderBook.clear();
    Order buyLimitOrder1 = Order::CreateLimitOrder(Side::BUY, orderBook.nextOrderId(), 3.50, 31);
    Order sellLimitOrder1 = Order::CreateLimitOrder(Side::SELL, orderBook.nextOrderId(), 4.92, 34);
    orderBook.submitOrder(buyLimitOrder1);
    orderBook.submitOrder(sellLimitOrder1);

    REQUIRE(orderBook.getMarketSpread() == Approx(1.42).epsilon(EPSILON));
}

// Test 3: Best Bid/Ask Methods
TEST_CASE("OrderBook - Best Bid/Ask Methods", "[tests]") {
    orderBook.clear();

    // Test best bid (only buy orders)
    orderBook.submitOrder(Order::CreateLimitOrder(Side::BUY, orderBook.nextOrderId(), 1.75, 50));

    REQUIRE(orderBook.getBestBidOrder()->getPrice() == Approx(1.75).epsilon(EPSILON));
    REQUIRE(orderBook.getBestAskOrder() == nullptr);

    orderBook.submitOrder(Order::CreateLimitOrder(Side::BUY, orderBook.nextOrderId(), 1.85, 50));

    REQUIRE(orderBook.getBestBidOrder()->getPrice() == Approx(1.85).epsilon(EPSILON));
    REQUIRE(orderBook.getBestAskOrder() == nullptr);

    orderBook.submitOrder(Order::CreateLimitOrder(Side::BUY, orderBook.nextOrderId(), 1.65, 50));

    REQUIRE(orderBook.getBestBidOrder()->getPrice() == Approx(1.85).epsilon(EPSILON));
    REQUIRE(orderBook.getBestAskOrder() == nullptr);

    orderBook.clear();
    // Test best ask (only sell orders)
    orderBook.submitOrder(Order::CreateLimitOrder(Side::SELL, orderBook.nextOrderId(), 1.65, 50));

    REQUIRE(orderBook.getBestAskOrder()->getPrice() == Approx(1.65).epsilon(EPSILON));
    REQUIRE(orderBook.getBestBidOrder() == nullptr);

    orderBook.submitOrder(Order::CreateLimitOrder(Side::SELL, orderBook.nextOrderId(), 1.55, 50));

    REQUIRE(orderBook.getBestAskOrder()->getPrice() == Approx(1.55).epsilon(EPSILON));
    REQUIRE(orderBook.getBestBidOrder() == nullptr);

    orderBook.submitOrder(Order::CreateLimitOrder(Side::SELL, orderBook.nextOrderId(), 1.75, 50));

    REQUIRE(orderBook.getBestAskOrder()->getPrice() == Approx(1.55).epsilon(EPSILON));
    REQUIRE(orderBook.getBestBidOrder() == nullptr);
}

// Test 4: Market Order Full Execution
TEST_CASE("MarketOrder - Execution Tests", "[tests]") {
    orderBook.clear();
    OrderId limOrderId = orderBook.nextOrderId();
    orderBook.submitOrder(Order::CreateLimitOrder(Side::SELL, limOrderId, 1.75, 100));
    orderBook.submitOrder(Order::CreateMarketOrder(Side::BUY, orderBook.nextOrderId(), 100));

    // The limit order should be completely filled and removed.
    REQUIRE(orderBook.getOrder(limOrderId) == nullptr);

    // And quantities at that price level should be zero.
    REQUIRE(orderBook.getAskQuantityAtLevel(1.75) == 0);
}

// Test 5: LimitOrder - No Execution When Prices Don't Cross
TEST_CASE("LimitOrder - No Execution When Prices Don't Cross", "[tests]") {
    orderBook.clear();
    Order buyOrder = Order::CreateLimitOrder(Side::BUY, orderBook.nextOrderId(), 1.70, 100);
    Order sellOrder = Order::CreateLimitOrder(Side::SELL, orderBook.nextOrderId(), 1.80, 100);
    orderBook.submitOrder(buyOrder);
    orderBook.submitOrder(sellOrder);

    // Orders remain in the book
    REQUIRE(orderBook.getOrder(buyOrder.getOrderId()) != nullptr);
    REQUIRE(orderBook.getOrder(sellOrder.getOrderId()) != nullptr);

    // Verify quantities at the levels.
    REQUIRE(orderBook.getBidQuantityAtLevel(1.70) == 100);
    REQUIRE(orderBook.getAskQuantityAtLevel(1.80) == 100);
}

// Test 6: LimitOrder - Immediate Execution When Prices Cross
TEST_CASE("LimitOrder - Immediate Execution When Prices Cross", "[tests]") {
    orderBook.clear();
    Order buyOrder = Order::CreateLimitOrder(Side::BUY, orderBook.nextOrderId(), 1.80, 100);
    Order sellOrder = Order::CreateLimitOrder(Side::SELL, orderBook.nextOrderId(), 1.75, 100);
    orderBook.submitOrder(buyOrder);
    orderBook.submitOrder(sellOrder);

    // Orders should match immediately and be removed.
    REQUIRE(orderBook.getOrder(buyOrder.getOrderId()) == nullptr);
    REQUIRE(orderBook.getOrder(sellOrder.getOrderId()) == nullptr);

    // Quantities at the price levels should be zero.
    REQUIRE(orderBook.getBidQuantityAtLevel(1.80) == 0);
    REQUIRE(orderBook.getAskQuantityAtLevel(1.75) == 0);
}

// Test 7: LimitOrder - Partial Fill
TEST_CASE("LimitOrder - Partial Fill", "[tests]") {
    orderBook.clear();
    Order sellOrder = Order::CreateLimitOrder(Side::SELL, orderBook.nextOrderId(), 2.00, 150);
    orderBook.submitOrder(sellOrder);

    // Submit a market order to buy 100 units.
    orderBook.submitOrder(Order::CreateMarketOrder(Side::BUY, orderBook.nextOrderId(), 100));

    // The sell order should be partially filled (remaining quantity = 50).
    OrderPointer remainingOrder = orderBook.getOrder(sellOrder.getOrderId());

    REQUIRE(remainingOrder != nullptr);
    REQUIRE(remainingOrder->getRemainingQuantity() == 50);

    // Verify the ask quantity at level 2.00 equals the remaining quantity.
    REQUIRE(orderBook.getAskQuantityAtLevel(2.00) == 50);
}

// Test 8: MarketOrder - Partial Fill Due To Insufficient Liquidity
TEST_CASE("MarketOrder - Partial Fill Due To Insufficient Liquidity", "[tests]") {
    orderBook.clear();
    Order sellOrder = Order::CreateLimitOrder(Side::SELL, orderBook.nextOrderId(), 2.00, 50);
    orderBook.submitOrder(sellOrder);

    // Submit a market order to buy 100 units.
    orderBook.submitOrder(Order::CreateMarketOrder(Side::BUY, orderBook.nextOrderId(), 100));

    // Sell order should be fully filled and removed.
    REQUIRE(orderBook.getOrder(sellOrder.getOrderId()) == nullptr);

    // Verify that the ask quantity is now zero.
    REQUIRE(orderBook.getAskQuantityAtLevel(2.00) == 0);
}

// Test 9: StopOrder - Buy Stop Not Triggered
TEST_CASE("StopOrder - Buy Stop Not Triggered", "[tests]") {
    orderBook.clear();

    // Submit a buy stop market order with stop price 1.90.
    Order stopBuyOrder = Order::CreateStopOrder(Side::BUY, orderBook.nextOrderId(), 1.90, 100);
    orderBook.submitOrder(stopBuyOrder);

    // Set market conditions that do NOT trigger the stop order.
    Order buyOrder = Order::CreateLimitOrder(Side::BUY, orderBook.nextOrderId(), 1.80, 100);
    Order sellOrder = Order::CreateLimitOrder(Side::SELL, orderBook.nextOrderId(), 1.95, 100);
    orderBook.submitOrder(buyOrder);
    orderBook.submitOrder(sellOrder);

    // Verify main book quantities.
    REQUIRE(orderBook.getBidQuantityAtLevel(1.80) == 100);
    REQUIRE(orderBook.getAskQuantityAtLevel(1.95) == 100);

    // Market price = (1.80+1.95)/2 = 1.875, less than stop price 1.90.
    // Stop order remains untriggered.
    REQUIRE(orderBook.getOrder(stopBuyOrder.getOrderId()) != nullptr);
}

// Test 10: StopOrder - Triggered Buy Stop Order
TEST_CASE("StopOrder - Triggered Buy Stop Order", "[tests]") {
    orderBook.clear();

    // Set up market condition with a sell order.
    Order sellOrder = Order::CreateLimitOrder(Side::SELL, orderBook.nextOrderId(), 1.75, 100);
    orderBook.submitOrder(sellOrder);

    // Submit a buy stop market order with stop price 1.70.
    Order stopBuyOrder = Order::CreateStopOrder(Side::BUY, orderBook.nextOrderId(), 1.70, 100);
    orderBook.submitOrder(stopBuyOrder);

    // Market price becomes 1.75 (from the sell order), which triggers the stop order.
    // Verify that the triggered stop order is removed.
    REQUIRE(orderBook.getOrder(stopBuyOrder.getOrderId()) == nullptr);

    // And the sell order is also fully executed.
    REQUIRE(orderBook.getOrder(sellOrder.getOrderId()) == nullptr);

    // Verify quantities: no orders should remain at 1.75.
    REQUIRE(orderBook.getAskQuantityAtLevel(1.75) == 0);
}

// Test 11: StopOrder - Triggered Sell Stop Order
TEST_CASE("StopOrder - Triggered Sell Stop Order", "[tests]") {
    orderBook.clear();

    // Set up market condition with a buy order.
    Order buyOrder = Order::CreateLimitOrder(Side::BUY, orderBook.nextOrderId(), 1.70, 100);
    orderBook.submitOrder(buyOrder);

    // Submit a sell stop market order with stop price 1.75.
    Order stopSellOrder = Order::CreateStopOrder(Side::SELL, orderBook.nextOrderId(), 1.75, 100);
    orderBook.submitOrder(stopSellOrder);

    // Market price becomes 1.70 (from the buy order), triggering the sell stop order.
    REQUIRE(orderBook.getOrder(stopSellOrder.getOrderId()) == nullptr);
    // The buy order should be fully executed by the triggered order.
    REQUIRE(orderBook.getOrder(buyOrder.getOrderId()) == nullptr);
    // Verify that the bid quantity at 1.70 is now zero.
    REQUIRE(orderBook.getBidQuantityAtLevel(1.70) == 0);
}

// Test 12: StopLimitOrder - Not Triggered
TEST_CASE("StopLimitOrder - Not Triggered", "[tests]") {
    orderBook.clear();

    // Submit a stop limit order (buy side) with stop price 1.90 and limit price 1.80.
    Order stopLimitOrder = Order::CreateStopLimitOrder(Side::BUY, orderBook.nextOrderId(), 1.90, 1.80, 100);
    orderBook.submitOrder(stopLimitOrder);

    // Set market conditions that do NOT trigger the stop limit order.
    Order buyOrder = Order::CreateLimitOrder(Side::BUY, orderBook.nextOrderId(), 1.75, 100);
    Order sellOrder = Order::CreateLimitOrder(Side::SELL, orderBook.nextOrderId(), 1.95, 100);
    orderBook.submitOrder(buyOrder);
    orderBook.submitOrder(sellOrder);

    // Verify that the main book has only the buy and sell orders.
    REQUIRE(orderBook.getBidQuantityAtLevel(1.75) == 100);
    REQUIRE(orderBook.getAskQuantityAtLevel(1.95) == 100);

    // The stop limit order is still pending (in the stop container) and not visible in bids.
    REQUIRE(orderBook.getOrder(stopLimitOrder.getOrderId()) != nullptr);
}

// Test 13: StopLimitOrder - Triggered
TEST_CASE("StopLimitOrder - Triggered", "[tests]") {
    orderBook.clear();

    // Submit a stop limit order (buy side) with stop price 1.70 and limit price 1.68.
    Order stopLimitOrder = Order::CreateStopLimitOrder(Side::BUY, orderBook.nextOrderId(), 1.70, 1.68, 100);
    orderBook.submitOrder(stopLimitOrder);

    // Set market condition to trigger the stop order.
    // For buy stop, trigger if market price >= stop price.
    // Submit a sell limit order with price 1.72.
    Order sellOrder = Order::CreateLimitOrder(Side::SELL, orderBook.nextOrderId(), 1.72, 100);
    orderBook.submitOrder(sellOrder);

    // The stop limit order should trigger and convert to a buy limit order at 1.68.
    OrderPointer triggeredOrder = orderBook.getOrder(stopLimitOrder.getOrderId());
    REQUIRE(triggeredOrder != nullptr);
    REQUIRE(triggeredOrder->getPrice() == Approx(1.68).epsilon(EPSILON));

    // Verify that the triggered order's quantity is added to the bid side.
    REQUIRE(orderBook.getBidQuantityAtLevel(1.68) == 100);
}

// Test 14: Mixed Orders - Limit Orders on Both Sides Matching
TEST_CASE("Mixed Orders - Limit Orders on Both Sides Matching", "[tests]") {
    orderBook.clear();

    // Submit a buy limit order and a sell limit order that cross.
    Order buyOrder = Order::CreateLimitOrder(Side::BUY, orderBook.nextOrderId(), 2.00, 100);
    Order sellOrder = Order::CreateLimitOrder(Side::SELL, orderBook.nextOrderId(), 1.90, 100);
    orderBook.submitOrder(buyOrder);
    orderBook.submitOrder(sellOrder);

    // They should match immediately.
    REQUIRE(orderBook.getOrder(buyOrder.getOrderId()) == nullptr);
    REQUIRE(orderBook.getOrder(sellOrder.getOrderId()) == nullptr);

    // Quantities at those levels should be zero.
    REQUIRE(orderBook.getBidQuantityAtLevel(2.00) == 0);
    REQUIRE(orderBook.getAskQuantityAtLevel(1.90) == 0);
}

// Test 15: Extreme Case - Very Large Quantities
TEST_CASE("Extreme Case - Very Large Quantities", "[tests]") {
    orderBook.clear();
    const Quantity largeQty = 1000000000;
    Order sellOrder = Order::CreateLimitOrder(Side::SELL, orderBook.nextOrderId(), 5.00, largeQty);
    orderBook.submitOrder(sellOrder);

    // Submit a market order to buy the entire large quantity.
    orderBook.submitOrder(Order::CreateMarketOrder(Side::BUY, orderBook.nextOrderId(), largeQty));

    // The sell order should be fully executed and removed.
    REQUIRE(orderBook.getOrder(sellOrder.getOrderId()) == nullptr);
    REQUIRE(orderBook.getAskQuantityAtLevel(5.00) == 0);
}

// Test 16: Order Removal - Cancel Non-existent Order
TEST_CASE("Order Removal - Cancel Non-existent Order", "[tests]") {
    orderBook.clear();
    OrderId nonExistentOrderId = 9999;
    orderBook.removeOrder(nonExistentOrderId, false);
    // The order book remains unchanged (empty).
    REQUIRE(orderBook.getBestAskOrder() == nullptr);
    REQUIRE(orderBook.getBestBidOrder() == nullptr);
}

// Test 17: Limit Orders - FIFO Order Matching
TEST_CASE("Limit Orders - FIFO Order Matching", "[tests]") {
    orderBook.clear();
    // Submit two sell limit orders at the same price.
    Order sellOrder1 = Order::CreateLimitOrder(Side::SELL, orderBook.nextOrderId(), 2.00, 30);
    Order sellOrder2 = Order::CreateLimitOrder(Side::SELL, orderBook.nextOrderId(), 2.00, 70);
    orderBook.submitOrder(sellOrder1);
    orderBook.submitOrder(sellOrder2);

    // Submit a market order to buy 50 units.
    orderBook.submitOrder(Order::CreateMarketOrder(Side::BUY, orderBook.nextOrderId(), 50));

    // The first order (30 units) should be fully filled and removed.
    REQUIRE(orderBook.getOrder(sellOrder1.getOrderId()) == nullptr);

    // The second order should be partially filled (70 - 20 = 50 remaining).
    OrderPointer remainingOrder = orderBook.getOrder(sellOrder2.getOrderId());

    REQUIRE(remainingOrder != nullptr);
    REQUIRE(remainingOrder->getRemainingQuantity() == 50);
    REQUIRE(orderBook.getAskQuantityAtLevel(2.00) == 50);
}

// Test 18: Market Price Calculation with Single Side Orders
TEST_CASE("Market Price Calculation with Single Side Orders", "[tests]") {
    orderBook.clear();

    // Only a buy order exists.
    Order buyOrder = Order::CreateLimitOrder(Side::BUY, orderBook.nextOrderId(), 1.65, 100);
    orderBook.submitOrder(buyOrder);

    // Market price should be the buy order's price.
    REQUIRE(orderBook.getMarketPrice() == Approx(1.65).epsilon(EPSILON));
    REQUIRE(orderBook.getBidQuantityAtLevel(1.65) == 100);

    orderBook.clear();

    // Only a sell order exists.
    Order sellOrder = Order::CreateLimitOrder(Side::SELL, orderBook.nextOrderId(), 1.85, 100);
    orderBook.submitOrder(sellOrder);

    // Market price should be the sell order's price.
    REQUIRE(orderBook.getMarketPrice() == Approx(1.85).epsilon(EPSILON));
    REQUIRE(orderBook.getAskQuantityAtLevel(1.85) == 100);
}

// Test 19: Stop Orders - Multiple Orders at Same Price
TEST_CASE("Stop Orders - Multiple Orders at Same Price", "[tests]") {
    orderBook.clear();

    // Submit two buy stop market orders with the same stop price.
    Order stopBuyOrder1 = Order::CreateStopOrder(Side::BUY, orderBook.nextOrderId(), 1.70, 100);
    Order stopBuyOrder2 = Order::CreateStopOrder(Side::BUY, orderBook.nextOrderId(), 1.70, 200);
    orderBook.submitOrder(stopBuyOrder1);
    orderBook.submitOrder(stopBuyOrder2);

    // Set market condition to trigger these stop orders.
    Order sellOrder = Order::CreateLimitOrder(Side::SELL, orderBook.nextOrderId(), 1.75, 300);
    orderBook.submitOrder(sellOrder);

    // Both stop orders should trigger and be removed.
    REQUIRE(orderBook.getOrder(stopBuyOrder1.getOrderId()) == nullptr);
    REQUIRE(orderBook.getOrder(stopBuyOrder2.getOrderId()) == nullptr);

    // The sell order should be fully matched.
    REQUIRE(orderBook.getAskQuantityAtLevel(1.75) == 0);
}

// Test 20: Order Filling - Overfill Prevention
TEST_CASE("Order Filling - Overfill Prevention", "[tests]") {
    // Directly test the Order class's fill method for overfill protection.
    Order order = Order::CreateLimitOrder(Side::BUY, 1, 1.50, 100);
    // Valid fill.
    REQUIRE_NOTHROW(order.fill(50));
    // Attempt to overfill: filling 60 when only 50 remains should throw.
    REQUIRE_THROWS_AS(order.fill(60), std::logic_error);
}

// Test 21: Sequential Triggering with Different Stop Levels
TEST_CASE("Stop Orders - Sequential Triggering with Different Stop Levels", "[tests]") {
    // Create limit orders
    Order limitBuy1 = Order::CreateLimitOrder(Side::BUY, orderBook.nextOrderId(), 98, 100);
    Order limitBuy2 = Order::CreateLimitOrder(Side::BUY, orderBook.nextOrderId(), 97, 50);
    Order limitSell1 = Order::CreateLimitOrder(Side::SELL, orderBook.nextOrderId(), 102, 100);
    Order limitSell2 = Order::CreateLimitOrder(Side::SELL, orderBook.nextOrderId(), 103, 50);
    orderBook.submitOrder(limitBuy1);
    orderBook.submitOrder(limitBuy2);
    orderBook.submitOrder(limitSell1);
    orderBook.submitOrder(limitSell2);

    // Stop Orders
    Order stopBuy1 = Order::CreateStopOrder(Side::BUY, orderBook.nextOrderId(), 95, 100);
    orderBook.submitOrder(stopBuy1);
    // Should have been triggered - take from sell order 1
    REQUIRE(orderBook.getOrder(stopBuy1.getOrderId()) == nullptr);
    REQUIRE(orderBook.getBestAskOrder()->getPrice() == Approx(103).epsilon(EPSILON));
    REQUIRE(orderBook.getAskQuantityAtLevel(103) == Approx(50).epsilon(EPSILON));

    Order stopBuy2 = Order::CreateStopOrder(Side::BUY, orderBook.nextOrderId(), 101, 100);
    orderBook.submitOrder(stopBuy2);
    // Should still be in the book
    REQUIRE(orderBook.getOrder(stopBuy2.getOrderId()) != nullptr);

    Order stopSell1 = Order::CreateStopOrder(Side::SELL, orderBook.nextOrderId(), 104, 100);
    orderBook.submitOrder(stopSell1);
    REQUIRE(orderBook.getOrder(stopSell1.getOrderId()) == nullptr);
    REQUIRE(orderBook.getBestBidOrder()->getPrice() == Approx(97).epsilon(EPSILON));
    REQUIRE(orderBook.getBidQuantityAtLevel(97) == Approx(50).epsilon(EPSILON));

    Order stopSell2 = Order::CreateStopOrder(Side::SELL, orderBook.nextOrderId(), 99, 100);
    REQUIRE(orderBook.getOrder(stopSell2.getOrderId()) == nullptr);
}