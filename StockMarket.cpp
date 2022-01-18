#include "StockMarket.h"
#include <iostream>
#include <string>
#include <utility>


// ================== StockMarket Class ==================

StockMarket::StockMarket(float lastTradePrice) {
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

void StockMarket::addOrder(Order order) {
    if(order.getAction() == 'B'){
        buyOrders_.push_back(order);
    }
    else if (order.getAction() == 'S'){
        sellOrders_.push_back(order);
    } else
        throw invalid_argument("This action for order is not supported");
}

vector<pair<Order,Order>> StockMarket::matchOrders() {
    vector<pair<Order,Order>> matches;
    if(getBuyOrders().empty() || getSellOrders().empty()){
        // DO NOTHING, since there's nothing to be matched against
        return matches;
    } else {
        for(auto buy = std::begin(buyOrders_); buy != std::end(buyOrders_); ++buy) {
            for(auto sell = std::begin(sellOrders_); sell != std::end(sellOrders_); ++sell) {
                if(buy->getLimitPrice()>= sell->getLimitPrice() || (buy->getType() == 'M' || sell->getType() == 'M')){
                    //Both Indivisible
                    if(buy->getDiv() == 'I' && sell->getDiv() == 'I' && buy->getQuantity() == sell->getQuantity()){
//                        matches.push_back(make_pair(buy,sell));
//                        pair<Order,Order> m = make_pair(buy,sell);


                    }
                }
            }
        }

        return matches;
    }
}

// ================== Order Class ==================

Order::Order(int age, string orderID, char action, char type, char div,int quantity, float limitPrice){
    age_ = age;
    orderID_ = orderID;
    action_ = action;
    type_ = type;
    div_ = div;
    quantity_ = quantity;
    limitPrice_ = limitPrice;

}

const string &Order::getOrderId() const {
    return orderID_;
}

char Order::getType() const {
    return type_;
}

int Order::getAge() const {
    return age_;
}

char Order::getAction() const {
    return action_;
}

char Order::getDiv() const {
    return div_;
}

int Order::getQuantity() const {
    return quantity_;
}


float Order::getLimitPrice() const {
    return limitPrice_;
};

// ================== Limit Order Class ==================

LimitOrder::LimitOrder(int age, string orderId, char action, char type, char div,int quantity, float limitPrice) : Order(age, orderId, action, type, div, quantity, limitPrice) {

    limitPrice_ = limitPrice;
    quantity_ = quantity;
}

float LimitOrder::getLimitPrice() const {
    return limitPrice_;
}

int LimitOrder::getQuantity() const {
    return quantity_;
}

test::test(string text) {
    message = text;
}
