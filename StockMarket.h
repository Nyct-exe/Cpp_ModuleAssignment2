#ifndef STOCKMARKET_H_
#define STOCKMARKET_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>


using namespace std;

class Order;

class StockMarket {

public:
    StockMarket(float lastTradePrice);

    // Getters
    const vector<Order> &getBuyOrders() const;

    const vector<Order> &getSellOrders() const;

    float getLastTradePrice() const;

    // Adds an order to sell/orders vector depending on action of an order
    void addOrder(Order order);

    // Match orders that could potentially be executed and returns a vector of pairs
    vector<pair<Order,Order>> matchOrders();

private:
    float lastTradePrice_;
    vector<Order> buyOrders_;
    vector<Order> sellOrders_;

};

class Order {

public:


    const string &getOrderId() const;

    char getType() const;

    char getAction() const;

    int getAge() const;

    char getDiv() const;

    int getQuantity() const;

    virtual float getLimitPrice() const;

protected:
    string orderID_;
    // Is it a buy or a sell
    char action_;
    char type_;
    char div_;
    // age of the older
    int age_;
    int quantity_;
    float limitPrice_;
    // Order Constructor
    Order(int age, string orderID, char action, char type, char div,int quantity, float limitPrice = 0);

};

class LimitOrder : public Order{

public:
    // Constructor of an Order
    LimitOrder(int age, string orderId, char action, char type, char div,int quantity, float limitPrice);

    float getLimitPrice() const override;

    int getQuantity() const;

private:
};

class test {
public:
    test(string text);
    string message;
};

#endif //STOCKMARKET_H_