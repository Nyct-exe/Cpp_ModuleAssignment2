#include "StockMarket.h"
#include <iostream>
#include <string>

// ================== StockMarket Class ==================
StockMarket::StockMarket(vector<Order> buyOrders, vector<Order> sellOrders) {
    buyOrders_ = buyOrders;
    sellOrders_ = sellOrders;
}


// ================== Order Class ==================

Order::Order(vector<Order> buyOrders, vector<Order> sellOrders, string orderID, char action, char type, char div)
        : StockMarket(buyOrders, sellOrders) {
    orderID_ = orderID;
    action_ = action;
    type_ = type;
    div_ = div;

};

// ================== Limit Order Class ==================

LimitOrder::LimitOrder(string orderId, char action, char type, char div, float limitPrice, int quantity) : Order(
        vector<Order>(), vector<Order>(), orderId, action, type, div) {

    limitPrice_ = limitPrice;
    quantity_ = quantity;
}

test::test(string text) {
    message = text;
}
