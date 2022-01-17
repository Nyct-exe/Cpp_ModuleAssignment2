#include "StockMarket.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

StockMarket createMarketFromData(string& file){

    string text;

    // Order Variables
    string orderID;
    char action;
    char type;
    char div;
    float price;
    int quantity;

    string lastOrder;

    // Create Order oop
    vector<Order> buyOrders;
    vector<Order> sellOrders;

    ifstream File(file);


    float lastPrice;
    File >> lastPrice;
    while (getline(File, text)){
        File >> orderID;
        File >> action;
        File >> type;
        File >> div;

        //Limit Order
        if(type == 'L'){
            File >> price;
            File >> quantity;

            // Buy Order
            if(action == 'B' && lastOrder != orderID){
                lastOrder = orderID;
                buyOrders.push_back(LimitOrder(orderID,action,type,div,price,quantity));
            }
            else if (action == 'S' && lastOrder != orderID){
                lastOrder = orderID;
                sellOrders.push_back(LimitOrder(orderID,action,type,div,price,quantity));
            }
        }

        // Market Order
        else if(type == 'M') {
            // DO SMTH
        }

    }

    File.close();
//    for(int i=0;i != sellOrders.size(); i++){
//        cout << sellOrders[i].orderID_ << endl;
//    }
    return StockMarket(buyOrders,sellOrders, lastPrice);
}

int main (int argc, char* argv[]) {
    // Todo: Error handling for reading files
    string fileName(argv[1]);
    // Creating a Stock Market based on data from the file
    StockMarket market = createMarketFromData(fileName);


}
