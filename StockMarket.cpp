#include "StockMarket.h"
#include <iostream>
#include <string>

// ================== StockMarket Class ==================
StockMarket::StockMarket(vector<Order> buyOrders, vector<Order> sellOrders, float lastTradePrice) {
    buyOrders_ = buyOrders;
    sellOrders_ = sellOrders;
    lastTradePrice_ = lastTradePrice;
}

const vector<Order> &StockMarket::getBuyOrders() const {
    return buyOrders_;
}

const vector<Order> &StockMarket::getSellOrders() const {
    return sellOrders_;
}

float StockMarket::getLastTradePrice() const {
    return lastTradePrice_;
}


// ================== Order Class ==================

Order::Order(string orderID, char action, char type, char div){
    orderID_ = orderID;
    action_ = action;
    type_ = type;
    div_ = div;

};

// ================== Limit Order Class ==================

LimitOrder::LimitOrder(string orderId, char action, char type, char div, float limitPrice, int quantity) : Order(orderId, action, type, div) {

    limitPrice_ = limitPrice;
    quantity_ = quantity;
}

test::test(string text) {
    message = text;
}
