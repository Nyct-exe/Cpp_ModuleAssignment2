#include "StockMarket.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

//void createMarketFromData(string& file){
//
//    string text;
//    int age = 0;
//
//    // Order Variables
//    string orderID;
//    char action;
//    char type;
//    char div;
//    float price;
//    int quantity;
//
//    string lastOrder;
//
//    ifstream File(file);
//
//
//    float lastPrice;
//    File >> lastPrice;
//    // Creating an instace of stock market to store buy/sell orders
//    StockMarket stockMarket = StockMarket(lastPrice);
//
//
//    while (getline(File, text)){
//        File >> orderID;
//        File >> action;
//        File >> type;
//        File >> div;
//
//        //Limit Order
//        if(type == 'L' && lastOrder != orderID){
//            lastOrder = orderID;
//            File >> price;
//            File >> quantity;
//            stockMarket.addOrder(LimitOrder(orderID,action,type,div,price,quantity,age));
//        }
//
//        // Market Order
//        else if(type == 'M') {
//            // DO SMTH
//        }
//        age++;
//
//    }
//
//    File.close();
//    for(int i=0;i != stockMarket.getSellOrders().size(); i++){
//        cout << stockMarket.getSellOrders()[i].getOrderId() << endl;
//    }
//}

int main (int argc, char* argv[]) {
    // Todo: Error handling for reading files
    string fileName(argv[1]);
    // Creating a Stock Market based on data from the file
//    StockMarket market = createMarketFromData(fileName);

    string text;
    int age = 0;

    // Order Variables
    string orderID;
    char action;
    char type;
    char div;
    float price;
    int quantity;

    string lastOrder;

    ifstream File(fileName);


    float lastPrice;
    File >> lastPrice;
    // Creating an instace of stock market to store buy/sell orders
    StockMarket stockMarket = StockMarket(lastPrice);


    while (getline(File, text)){
        File >> orderID;
        File >> action;
        File >> type;
        File >> div;

        //Limit Order
        if(type == 'L' && lastOrder != orderID){
            lastOrder = orderID;
            File >> price;
            File >> quantity;
            stockMarket.addOrder(LimitOrder(age,orderID,action,type,div,quantity,price));
        }

            // Market Order
        else if(type == 'M') {
            // DO SMTH
        }
        age++;

        // Match Orders


    }

    File.close();
    // For Testing

    for(int i=0;i != stockMarket.getSellOrders().size(); i++){
        cout << stockMarket.getSellOrders()[i].getOrderId() << endl;
    }


}
