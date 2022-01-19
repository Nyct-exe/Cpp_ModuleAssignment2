#include "StockMarket.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <utility>
#include <memory>
#include <algorithm>
#include <fstream>

// ================== StockMarket Class ==================

StockMarket::StockMarket(float lastTradePrice) {
    lastTradePrice_ = lastTradePrice;
}

const vector<shared_ptr<struct Order>> StockMarket::getBuyOrders() const {
    return buyOrders_;
}

const vector<shared_ptr<struct Order>> StockMarket::getSellOrders() const {
    return sellOrders_;
}

float StockMarket::getLastTradePrice() const {
    return lastTradePrice_;
}

void StockMarket::addOrder(shared_ptr<Order> order) {
    if(order->getAction() == 'B'){
        buyOrders_.push_back(order);
    }
    else if (order->getAction() == 'S'){
        sellOrders_.push_back(order);
    } else
        throw invalid_argument("This action for order is not supported");
}

vector<pair<shared_ptr<Order>,shared_ptr<Order>>> StockMarket::matchOrders() {

    vector<pair<shared_ptr<Order> ,shared_ptr<Order>>> matches;
    if(getBuyOrders().empty() || getSellOrders().empty()){
        // DO NOTHING, since there's nothing to be matched against
        return matches;
    } else {
        for(auto buy = std::begin(buyOrders_); buy != std::end(buyOrders_); ++buy) {
            for(auto sell = std::begin(sellOrders_); sell != std::end(sellOrders_); ++sell) {
                if(buy->get()->getLimitPrice()>= sell->get()->getLimitPrice() || (buy->get()->getType() == 'M' || sell->get()->getType() == 'M')){
                    //Both Indivisible
                    if(buy->get()->getDiv() == 'I' && sell->get()->getDiv() == 'I'
                       && buy->get()->getQuantity() == sell->get()->getQuantity()){

                        matches.push_back((make_pair(*buy,*sell)));
                    }
                    else if ((buy->get()->getDiv() == 'I' && sell->get()->getDiv() == 'D')
                             && sell->get()->getQuantity() >= buy->get()->getQuantity()){
                        matches.push_back((make_pair(*buy,*sell)));
                    }
                    else if ((buy->get()->getDiv() == 'D' && sell->get()->getDiv() == 'I')
                             && sell->get()->getQuantity() <= buy->get()->getQuantity()) {
                        matches.push_back((make_pair(*buy,*sell)));
                    }
                    else if (buy->get()->getDiv() == 'D' && sell->get()->getDiv() == 'D') {
                        matches.push_back((make_pair(*buy,*sell)));
                    }
                }
            }
        }
        // Needs further testing
//        // Making sure Matches are matched according to priority
//        for(auto it = std::begin(matches); it != std::end(matches); ++it) {
//            for(auto rit = std::rbegin(matches); rit != std::rend(matches); ++rit) {
//                //consider changing
//                pair<Order*, Order*> firstPair = make_pair(it->first,it->second);
//                if((it->first == rit->first) && (it->second != rit->second)){
//                    // DO Nothing For now
//                }
//            }
//        }
//        return matches;
    }
}

void StockMarket::executeOrders(vector<pair<shared_ptr<Order>,shared_ptr<Order>>>  matches) {
    // Stores executed actions

    if (!matches.empty()) {
        for (auto it = std::begin(matches); it != std::end(matches); ++it) {
            if (it->first->getType() == 'L' && it->second->getType() == 'L') {
                if (it->first->getAge() < it->second->getAge()) {
                    executionLogs_ << "order " << it->first->getOrderId() << " " << it->first->getQuantity()
                                   << " shares purchased at price " << it->first->getLimitPrice() << fixed << setprecision(2) << endl;
                    executionLogs_ << "order " << it->second->getOrderId() << " " << it->second->getQuantity()
                                   << " shares sold at price " << it->first->getLimitPrice() << fixed << setprecision(2) << endl;
                    lastTradePrice_ = it->first->getLimitPrice();
                }
                else if (it->first->getAge() > it->second->getAge()) {
                    executionLogs_ << "order " << it->first->getOrderId() << " " << it->first->getQuantity()
                                   << " shares purchased at price " << it->second->getLimitPrice() << fixed << setprecision(2) << endl;
                    executionLogs_ << "order " << it->second->getOrderId() << " " << it->second->getQuantity()
                                   << " shares sold at price " << it->second->getLimitPrice() << fixed << setprecision(2) << endl;
                    lastTradePrice_ = it->second->getLimitPrice();
                }
            }
        }
    }
}

void StockMarket::removeMatches(vector<pair<shared_ptr<Order>,shared_ptr<Order>>> matchList) {
    if(!matchList.empty())
        for(auto match = std::begin(matchList); match != std::end(matchList); ++match) {
            buyOrders_.erase(remove(buyOrders_.begin(),buyOrders_.end(), match->first), buyOrders_.end());
            sellOrders_.erase(remove(sellOrders_.begin(), sellOrders_.end(),match->second), sellOrders_.end());
        }
    matchList.clear();
}

void StockMarket::terminalOutput() {

    stringstream buyString;
    stringstream sellString;
    for(auto buy = std::begin(buyOrders_); buy != std::end(buyOrders_); ++buy) {
        for(auto sell = std::begin(sellOrders_); sell != std::end(sellOrders_); ++sell) {
        }
    }
    cout << buyString.str();

}

void StockMarket::fileOutput() {

    ofstream File("executions.txt");

    File <<  executionLogs_.str();

    if(!buyOrders_.empty())
        for(auto it = std::begin(buyOrders_); it != std::end(buyOrders_); ++it)
            File << "order " << it->get()->getOrderId() << " " << it->get()->getQuantity() << " shares unexecuted" << "\n";
    if(!sellOrders_.empty())
        for(auto it = std::begin(sellOrders_); it != std::end(sellOrders_); ++it)
            File << "order " << it->get()->getOrderId() << " " << it->get()->getQuantity() << " shares unexecuted" << "\n";

    File.close();

}

// ================== Order Class ==================

Order::Order(int age, string orderID, char action, char type, char div,int quantity, float limitPrice){
    age_ = age;
    orderID_ = orderID;
    action_ = action;
    type_ = type;
    div_ = div;
    quantity_ = quantity;
    limitPrice_ = limitPrice;

}

const string &Order::getOrderId() const {
    return orderID_;
}

char Order::getType() const {
    return type_;
}

int Order::getAge() const {
    return age_;
}

char Order::getAction() const {
    return action_;
}

char Order::getDiv() const {
    return div_;
}

int Order::getQuantity() const {
    return quantity_;
}


float Order::getLimitPrice() const {
    return limitPrice_;
};
