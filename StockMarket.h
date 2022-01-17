#ifndef STOCKMARKET_H_
#define STOCKMARKET_H_

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Market {

public:
};

class LimitOrder {

public:
    // Constructor of an Order
    LimitOrder(string orderID, char action, char type, char div, float limitPrice, int quantity);

    string orderID_;
private:
    char action_;
    char type_;
    char div_;
    float limitPrice_;
    int quantity_;
};

class test {
public:
    test(string text);
    string message;
};

#endif //STOCKMARKET_H_