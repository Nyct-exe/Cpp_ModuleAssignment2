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

const deque<shared_ptr<struct Order>> StockMarket::getBuyOrders() const {
    return buyOrders_;
}

const deque<shared_ptr<struct Order>> StockMarket::getSellOrders() const {
    return sellOrders_;
}

float StockMarket::getLastTradePrice() const {
    return lastTradePrice_;
}

void StockMarket::addOrder(shared_ptr<Order> order) {

    if(order->getAction() == 'B'){
        if(!buyOrders_.empty()) {
            if ((order->getLimitPrice() > buyOrders_.front()->getLimitPrice()) || (order->getType() == 'M'))
                buyOrders_.push_front(order);
            else
                buyOrders_.push_back(order);
        } else {
            buyOrders_.push_back(order);
        }
    }
    else if (order->getAction() == 'S'){
        if(!sellOrders_.empty()){
            if((order->getLimitPrice() < sellOrders_.front()->getLimitPrice()) || (order->getType() == 'M' ))
                sellOrders_.push_front(order);
            else
                sellOrders_.push_back(order);
        } else {
            sellOrders_.push_back(order);
        }

    }
    // A Deque Sorted by Age (Needed for Output)
    ordersByAge.push_back(order);
}

deque<pair<shared_ptr<Order>,shared_ptr<Order>>> StockMarket::matchOrders() {

    deque<pair<shared_ptr<Order> ,shared_ptr<Order>>> matches;
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
    return matches;
    }
}

void StockMarket::executeOrders(deque<pair<shared_ptr<Order>,shared_ptr<Order>>>  matches) {


    if (!matches.empty()) {
        for (auto it = std::begin(matches); it != std::end(matches); ++it) {

            // Remembering Quantities initially
           int buyQuantity = it->first->getQuantity();
           int sellQuantity = it->second->getQuantity();

           int purchaseQuantity;

            // Limited Orders
            if (it->first->getType() == 'L' && it->second->getType() == 'L') {
                // InDivisible
                if(it->first->getDiv() == 'I' && it->second->getDiv() == 'I'){

                    // If Equal Limit Prices Do It based on age
//                    if(it->first->getLimitPrice() == it->second->getLimitPrice()){
                    if(buyQuantity < sellQuantity)
                        purchaseQuantity = buyQuantity;
                    else
                        purchaseQuantity = sellQuantity;

                    if ((it->first->getAge() < it->second->getAge())) {
                        executionLogs_ << "order " << it->first->getOrderId() << " " << purchaseQuantity
                                       << " shares purchased at price " << it->first->getLimitPrice() << fixed << setprecision(2) << endl;

                        executionLogs_ << "order " << it->second->getOrderId() << " " << purchaseQuantity
                                       << " shares sold at price " << it->first->getLimitPrice() << fixed << setprecision(2) << endl;
                        lastTradePrice_ = it->first->getLimitPrice();

                    }
                    else if (it->first->getAge() > it->second->getAge()) {
                        executionLogs_ << "order " << it->first->getOrderId() << " " << purchaseQuantity
                                       << " shares purchased at price " << it->second->getLimitPrice() << fixed << setprecision(2) << endl;
                        executionLogs_ << "order " << it->second->getOrderId() << " " << purchaseQuantity
                                       << " shares sold at price " << it->second->getLimitPrice() << fixed << setprecision(2) << endl;
                        lastTradePrice_ = it->second->getLimitPrice();

                    }

                    // Removing sold/bought Stock
                    it->first->setQuantity(0);
                    it->second->setQuantity(0);
                        // Not Equal Limit Prices
//                    } else {
//                            //TODO
//
//                    }
                    // Divisible
                } else {

                    // If Equal Limit Prices Do It based on age
//                    if(it->first->getLimitPrice() == it->second->getLimitPrice()){
                        if(buyQuantity < sellQuantity)
                            purchaseQuantity = buyQuantity;
                        else
                            purchaseQuantity = sellQuantity;

                        if ((it->first->getAge() < it->second->getAge())) {
                            executionLogs_ << "order " << it->first->getOrderId() << " " << purchaseQuantity
                                           << " shares purchased at price " << it->first->getLimitPrice() << fixed << setprecision(2) << endl;

                            executionLogs_ << "order " << it->second->getOrderId() << " " << purchaseQuantity
                                           << " shares sold at price " << it->first->getLimitPrice() << fixed << setprecision(2) << endl;
                            lastTradePrice_ = it->first->getLimitPrice();

                        }
                        else if (it->first->getAge() > it->second->getAge()) {
                            executionLogs_ << "order " << it->first->getOrderId() << " " << purchaseQuantity
                                           << " shares purchased at price " << it->second->getLimitPrice() << fixed << setprecision(2) << endl;
                            executionLogs_ << "order " << it->second->getOrderId() << " " << purchaseQuantity
                                           << " shares sold at price " << it->second->getLimitPrice() << fixed << setprecision(2) << endl;
                            lastTradePrice_ = it->second->getLimitPrice();

                        }
                        // Removing Quantity sold/bought

                        if(buyQuantity > sellQuantity) {
                            it->first.get()->setQuantity(buyQuantity - sellQuantity);
                            it->second.get()->setQuantity(0);
                        } else {
                            it->first.get()->setQuantity(0);
                            it->second.get()->setQuantity(sellQuantity - buyQuantity);
                        }




//                    }

                }


            }
        }
    }
}

void StockMarket::removeMatches(deque<pair<shared_ptr<Order>,shared_ptr<Order>>> matchList) {
    if(!matchList.empty())
        for(auto match = std::begin(matchList); match != std::end(matchList); ++match) {
            if(match->first->getType() == 'L' && match->first->getQuantity() == 0)
                buyOrders_.erase(remove(buyOrders_.begin(),buyOrders_.end(), match->first), buyOrders_.end());
            if(match->second->getType() == 'L' && match->second->getQuantity() == 0)
                sellOrders_.erase(remove(sellOrders_.begin(), sellOrders_.end(),match->second), sellOrders_.end());
        }
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


    // File Output

    if(!ordersByAge.empty())
        for(auto it = std::begin(ordersByAge); it != std::end(ordersByAge); ++it)
            if(it->get()->getQuantity() != 0)
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
}

void Order::setLimitPrice(float limitPrice) {
    limitPrice_ = limitPrice;
}

void Order::setQuantity(int quantity) {
    quantity_ = quantity;
};
