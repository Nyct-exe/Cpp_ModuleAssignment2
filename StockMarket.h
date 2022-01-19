#ifndef NEWLIFE_STOCKMARKET_H
#define NEWLIFE_STOCKMARKET_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <sstream>

using namespace std;

class Order;

class StockMarket {

public:
    StockMarket(float lastTradePrice);

    // Getters/Setters
    const vector<shared_ptr<struct Order>> getBuyOrders() const;

    const vector<shared_ptr<struct Order>> getSellOrders() const;

    float getLastTradePrice() const;

    void setLastTradePrice(float lastTradePrice);

    // Adds an order to sell/orders vector depending on action of an order
    void addOrder(shared_ptr<Order> order);

    // Removes Matches from the buy/sell orders
    void removeMatches(vector<pair<shared_ptr<Order>,shared_ptr<Order>>> matchList);

    // Match orders that could potentially be executed and returns a vector of pairs
    vector<pair<shared_ptr<Order>,shared_ptr<Order>>>  matchOrders();

    //Executes Orders that are matched
    void executeOrders(vector<pair<shared_ptr<Order>,shared_ptr<Order>>> matches);

    //Outputs to File
    void fileOutput();


    // Generates Output to the Terminal
    void terminalOutput();

private:
    float lastTradePrice_;
    stringstream executionLogs_;
    vector<shared_ptr<Order>> buyOrders_;
    vector<shared_ptr<Order>> sellOrders_;

};

class Order {

public:

    Order(int age, string orderID, char action, char type, char div,int quantity, float limitPrice = 0);

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

};

#endif //NEWLIFE_STOCKMARKET_H
