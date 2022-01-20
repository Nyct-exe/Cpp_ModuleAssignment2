#ifndef NEWLIFE_STOCKMARKET_H
#define NEWLIFE_STOCKMARKET_H

#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <utility>
#include <memory>
#include <sstream>
#include <deque>

using namespace std;

class Order;

class StockMarket {

public:
    StockMarket(float lastTradePrice);

    // Getters/Setters
    const deque<shared_ptr<struct Order>> getBuyOrders() const;

    const deque<shared_ptr<struct Order>> getSellOrders() const;

    float getLastTradePrice() const;

    // Adds an order to sell/orders deque depending on action of an order, also makes sure that Market orders are aded to the front by age
    void addOrder(shared_ptr<Order> order);

    // Removes Matches from the buy/sell orders
    void removeMatches(deque<pair<shared_ptr<Order>, shared_ptr<Order>>> matchList);

    // Match orders that could potentially be executed and returns a deque of pairs
    deque<pair<shared_ptr<Order>, shared_ptr<Order>>> matchOrders();

    //Executes Orders that are matched
    void executeOrders(deque<pair<shared_ptr<Order>, shared_ptr<Order>>> matches);

    //Outputs to File
    void fileOutput();

private:
    float lastTradePrice_;
    // text from execution
    stringstream executionLogs_;
    // Stores all buy orders
    deque<shared_ptr<Order>> buyOrders_;
    // Stores all sell orders
    deque<shared_ptr<Order>> sellOrders_;
    // Stores all orders that are coming in and sorts by age
    deque<shared_ptr<Order>> ordersByAge;

};

class Order {

public:
    // Order Constructor
    Order(int age, string orderID, char action, char type, char div, int quantity, float limitPrice = 0);

    // Getters/Setters
    const string &getOrderId() const;

    char getType() const;

    char getAction() const;

    int getAge() const;

    char getDiv() const;

    int getQuantity() const;

    float getLimitPrice() const;

    void setQuantity(int quantity);


//protected:
private:
    string orderID_;
    // Is it a buy or a sell
    char action_;
    char type_;
    char div_;
    // age of the older
    int age_;
    int quantity_;
    float limitPrice_;

};

#endif //NEWLIFE_STOCKMARKET_H
