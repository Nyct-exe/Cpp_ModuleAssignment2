#include "StockMarket.h"
#include <string>
#include <deque>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {

    // Order Variables
    string orderID;
    char action;
    char type;
    char div;
    float price;
    int quantity;
    int age = 0;

    // stores file text
    string text;

    string executionLog;

    string lastOrder;
    float lastPrice;


    //Opens a file to read
    ifstream File (argv[1]);
    File >> lastPrice;

    // Creating an instance of stock market to store buy/sell orders
    StockMarket stockMarket = StockMarket (lastPrice);
    while (getline (File, text)) {
        File >> orderID;
        File >> action;
        File >> type;
        File >> div;
        // Only gets price if its a Limit Order
        if (type == 'L')
            File >> price;
        else
            price = stockMarket.getLastTradePrice ();
        File >> quantity;
        if (orderID != lastOrder) {
            stockMarket.addOrder (make_shared<Order> (Order (age, orderID, action, type, div, quantity, price)));
            lastOrder = orderID;
        }

        age++;

        deque<pair<shared_ptr<Order>, shared_ptr<Order>>> matches = stockMarket.matchOrders ();


        stockMarket.executeOrders (matches);

        stockMarket.removeMatches (matches);


    }

    File.close ();

    stockMarket.fileOutput ();

    cout << "executions.txt, has been generated" << endl;


}