#ifndef STOCKMARKET_H_
#define STOCKMARKET_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Order;

class StockMarket {

public:
    StockMarket(vector<Order> buyOrders, vector<Order> sellOrders, float lastTradePrice);

    const vector<Order> &getBuyOrders() const;

    const vector<Order> &getSellOrders() const;

    float getLastTradePrice() const;

private:
    float lastTradePrice_;
    vector<Order> buyOrders_;
    vector<Order> sellOrders_;

};

class Order {

public:

protected:
    string orderID_;
    char action_;
    char type_;
    char div_;

    Order(string orderID, char action, char type, char div);

};

class LimitOrder : public Order{

public:
    // Constructor of an Order
    LimitOrder(string orderId, char action, char type, char div, float limitPrice, int quantity);
private:
    float limitPrice_;
    int quantity_;
};

class test {
public:
    test(string text);
    string message;
};

#endif //STOCKMARKET_H_