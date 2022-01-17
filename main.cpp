#include "StockMarket.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

void read_file(string& file){

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
    vector<LimitOrder> buyOrders;
    vector<LimitOrder> sellOrders;

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
        }
        // Market Order
        else if(type == 'M') {
            // DO SMTH
        }

//        // Buy Order
        if(action == 'B' && lastOrder != orderID){
            lastOrder = orderID;
            buyOrders.push_back(LimitOrder(orderID,'a','b','c',price,quantity));
        }
        else if (action == 'S' && lastOrder != orderID){
            lastOrder = orderID;
            sellOrders.push_back(LimitOrder(orderID,action,type,div,price,quantity));
        }
    }

    File.close();
    for(int i=0;i != sellOrders.size(); i++){
        cout << sellOrders[i].orderID_ << endl;
    }
}

int main (int argc, char* argv[]) {
    string fileName(argv[1]);
    read_file(fileName);
}
