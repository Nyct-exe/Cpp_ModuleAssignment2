#include "StockMarket.h"
#include <string>
#include <deque>
#include <iostream>
#include <limits>
#include <iomanip>
#include <sstream>

using namespace std;

int main (int argc, char* argv[]) {

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
    ifstream File(argv[1]);
    File >>lastPrice;

    // Creating an instance of stock market to store buy/sell orders
    StockMarket stockMarket = StockMarket(lastPrice);
    while (getline(File, text)){
        File >> orderID;
        File >> action;
        File >> type;
        File >> div;
        // Only gets price if its a Limit Order
        if(type == 'L')
            File >> price;
        else
            price = stockMarket.getLastTradePrice();
        File >> quantity;
        if(orderID != lastOrder){
            stockMarket.addOrder(make_shared<Order>(Order(age,orderID,action,type,div,quantity,price)));
            lastOrder = orderID;
        }

        age++;

        //TODO: After adding the order to the stockmarket we need to match it with existing orders
        deque<pair<shared_ptr<Order>,shared_ptr<Order>>> matches = stockMarket.matchOrders();

//        if(!test.empty())
//            for(auto it = std::begin(test); it != std::end(test); ++it) {
//                std::cout << it->first.getOrderId() << " : " << it->second.getOrderId() << "\n";
//            }

//        cout << "latest price: " << stockMarket.getLastTradePrice() << fixed << setprecision(2) << endl;
//        cout << "buy" << "                       |" << " sell                 " << endl;
        cout << "=============================================" << endl;

        //TODO: Order info and processed text afterwards

//        stockMarket.terminalOutput();
        // Checking Matches
        for(auto it = begin(matches); it != end(matches); ++it)
            cout << it->first->getOrderId() << " : " << it->second->getOrderId() << endl;

        stockMarket.executeOrders(matches);

        stockMarket.removeMatches(matches);



//        cout << "Press 'Enter' to Continue...";
//        cin.ignore(std::numeric_limits<streamsize>::max(),'\n');


    }

    File.close();

    stockMarket.fileOutput();

//    cout << "TEST: " << stockMarket.getSellOrders()[0]->getOrderId() << endl;

}