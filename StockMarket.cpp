#include "StockMarket.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <utility>
#include <memory>
#include <algorithm>

// ================== StockMarket Class ==================

StockMarket::StockMarket(float lastTradePrice) {
    lastTradePrice_ = lastTradePrice;
    executionLogs_ << "";
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
    if ( order->getAction() == 'B' ) {
        if ( !buyOrders_.empty()) {
            if ((order->getLimitPrice() > buyOrders_.front()->getLimitPrice()) || (order->getType() == 'M'))
                buyOrders_.push_front(order);
            else if ((order->getLimitPrice() < buyOrders_.front()->getLimitPrice()) || (order->getType() != 'M'))
                buyOrders_.push_back(order);
        } else {
            buyOrders_.push_back(order);
        }
    } else if ( order->getAction() == 'S' ) {
        if ( !sellOrders_.empty()) {
            if ((order->getLimitPrice() < sellOrders_.front()->getLimitPrice()) || (order->getType() == 'M'))
                sellOrders_.push_front(order);
            else
                sellOrders_.push_back(order);
        } else {
            sellOrders_.push_back(order);
        }

    }
    // A Deque Sorted by Age (Needed for Output)
    ordersByAge_.push_back(order);
}

deque<pair<shared_ptr<Order>, shared_ptr<Order>>> StockMarket::matchOrders() {

    // Sorting Buy/sell orders Dequeues (Don't need to sort by age for Market types since on adding its always added to the front of dequeue)
    sort(buyOrders_.begin(), buyOrders_.end(), [](const shared_ptr<Order> &a, const shared_ptr<Order> &b) {
        if (((a->getLimitPrice() == b->getLimitPrice()) && (a->getType() != 'M' || b->getType() != 'M'))) {
            return a->getAge() < b->getAge();
        }
        if ((a->getType() != 'M' || b->getType() != 'M'))
            return (a->getLimitPrice() > b->getLimitPrice());

        return true;
    });

    sort(sellOrders_.begin(), sellOrders_.end(), [](const shared_ptr<Order> &a, const shared_ptr<Order> &b) {
        if (((a->getLimitPrice() == b->getLimitPrice()) && (a->getType() != 'M' || b->getType() != 'M'))) {
            return a->getAge() < b->getAge();
        }
        if ((a->getType() != 'M' || b->getType() != 'M'))
            return (a->getLimitPrice() < b->getLimitPrice());

        return true;
    });

    deque<pair<shared_ptr<Order>, shared_ptr<Order>>> matches;
    if ( getBuyOrders().empty() || getSellOrders().empty()) {
        // DO NOTHING, since there's nothing to be matched against
        return matches;
    } else {
        for (auto buy = std::begin(buyOrders_); buy != std::end(buyOrders_); ++buy) {
            for (auto sell = std::begin(sellOrders_); sell != std::end(sellOrders_); ++sell) {
                if ((buy->get()->getLimitPrice() >= sell->get()->getLimitPrice()) ||
                    (buy->get()->getType() == 'M' || sell->get()->getType() == 'M')) {
                    //Both Indivisible
                    if ((buy->get()->getDiv() == 'I' && sell->get()->getDiv() == 'I')
                        && (buy->get()->getQuantity() == sell->get()->getQuantity())) {

                        matches.push_back((make_pair(*buy, *sell)));
                    } else if ((buy->get()->getDiv() == 'I' && sell->get()->getDiv() == 'D')
                               && (sell->get()->getQuantity() >= buy->get()->getQuantity())) {

                        matches.push_back((make_pair(*buy, *sell)));
                    } else if ((buy->get()->getDiv() == 'D' && sell->get()->getDiv() == 'I')
                               && (sell->get()->getQuantity() <= buy->get()->getQuantity())) {

                        matches.push_back((make_pair(*buy, *sell)));
                    } else if ((buy->get()->getDiv() == 'D' && sell->get()->getDiv() == 'D')) {

                        matches.push_back((make_pair(*buy, *sell)));
                    }
                }
            }
        }
        return matches;
    }
}

void StockMarket::executeOrders(deque<pair<shared_ptr<Order>, shared_ptr<Order>>> matches) {

    executionLogs_ << fixed << setprecision(2);

    if ( !matches.empty()) {

        // In case there are orders that are completed but still try to execute.
        for (auto it = std::begin(matches); it != std::end(matches); ++it) {
            if ( it->first->getQuantity() == 0 || it->second->getQuantity() == 0 )
                break;
            // Remembering Quantities initially
            int buyQuantity = it->first->getQuantity();
            int sellQuantity = it->second->getQuantity();

            int purchaseQuantity;

            // Limited Orders
            if ( it->first->getType() == 'L' && it->second->getType() == 'L' ) {
                if ( buyQuantity < sellQuantity )
                    purchaseQuantity = buyQuantity;
                else
                    purchaseQuantity = sellQuantity;

                if ((it->first->getAge() < it->second->getAge())) {
                    executionLogs_ << "order " << it->first->getOrderId() << " " << purchaseQuantity
                                   << " shares purchased at price " << it->first->getLimitPrice() << endl;

                    executionLogs_ << "order " << it->second->getOrderId() << " " << purchaseQuantity
                                   << " shares sold at price " << it->first->getLimitPrice()
                                   << endl;
                    lastTradePrice_ = it->first->getLimitPrice();

                } else if ( it->first->getAge() > it->second->getAge()) {
                    executionLogs_ << "order " << it->first->getOrderId() << " " << purchaseQuantity
                                   << " shares purchased at price " << it->second->getLimitPrice() << endl;
                    executionLogs_ << "order " << it->second->getOrderId() << " " << purchaseQuantity
                                   << " shares sold at price " << it->second->getLimitPrice() << endl;
                    lastTradePrice_ = it->second->getLimitPrice();

                }
                // Removing Quantity sold/bought

                if ( buyQuantity >= sellQuantity ) {
                    it->first.get()->setQuantity(buyQuantity - sellQuantity);
                    it->second.get()->setQuantity(0);
                } else {
                    it->first.get()->setQuantity(0);
                    it->second.get()->setQuantity(sellQuantity - buyQuantity);
                }

            }
            // Market Order
            if ( it->first.get()->getType() == 'M' || it->second.get()->getType() == 'M' ) {

                if ( buyQuantity < sellQuantity )
                    purchaseQuantity = buyQuantity;
                else
                    purchaseQuantity = sellQuantity;

                if ( it->first.get()->getType() == 'M' && it->second->getType() == 'L' ) {
                    executionLogs_ << "order " << it->first->getOrderId() << " " << purchaseQuantity
                                   << " shares purchased at price " << it->second->getLimitPrice() << endl;

                    executionLogs_ << "order " << it->second->getOrderId() << " " << purchaseQuantity
                                   << " shares sold at price " << it->second->getLimitPrice() << endl;
                    lastTradePrice_ = it->second->getLimitPrice();

                } else if ( it->first->getType() == 'L' && it->second->getType() == 'M' ) {
                    executionLogs_ << "order " << it->first->getOrderId() << " " << purchaseQuantity
                                   << " shares purchased at price " << it->first->getLimitPrice() << endl;

                    executionLogs_ << "order " << it->second->getOrderId() << " " << purchaseQuantity
                                   << " shares sold at price " << it->first->getLimitPrice()
                                   << endl;
                    lastTradePrice_ = it->first->getLimitPrice();
                } else {
                    executionLogs_ << "order " << it->first->getOrderId() << " " << purchaseQuantity
                                   << " shares purchased at price " << lastTradePrice_
                                   << endl;

                    executionLogs_ << "order " << it->second->getOrderId() << " " << purchaseQuantity
                                   << " shares sold at price " << lastTradePrice_ << endl;
                }

                // Removing Quantity sold/bought

                if ( buyQuantity >= sellQuantity ) {
                    it->first.get()->setQuantity(buyQuantity - sellQuantity);
                    it->second.get()->setQuantity(0);
                } else {
                    it->first.get()->setQuantity(0);
                    it->second.get()->setQuantity(sellQuantity - buyQuantity);
                }

            }
        }
    }
}

void StockMarket::removeMatches(deque<pair<shared_ptr<Order>, shared_ptr<Order>>> matchList) {
    if ( !matchList.empty())
        for (auto match = std::begin(matchList); match != std::end(matchList); ++match) {
            if ( match->first->getType() == 'L' && match->first->getQuantity() == 0 )
                buyOrders_.erase(remove(buyOrders_.begin(), buyOrders_.end(), match->first), buyOrders_.end());
            if ( match->second->getType() == 'L' && match->second->getQuantity() == 0 )
                sellOrders_.erase(remove(sellOrders_.begin(), sellOrders_.end(), match->second), sellOrders_.end());
        }
}

void StockMarket::fileOutput() {

    ofstream File("executions.txt");

//    File << executionLogs_.str();
    // File Output
    // Outputs orders by age and ignores all orders that are completed ( quoantity = 0)
    if ( !ordersByAge_.empty())
        for (auto it = std::begin(ordersByAge_); it != std::end(ordersByAge_); ++it)
            if ( it->get()->getQuantity() != 0)
                executionLogs_ << "order " << it->get()->getOrderId() << " " << it->get()->getQuantity() << " shares unexecuted" << '\n';

    // Removes additional new line
    File << executionLogs_.str().substr(0, executionLogs_.str().size()-1);
    File.close();

}

// ================== Order Class ==================

Order::Order(int age, string orderID, char action, char type, char div, int quantity, float limitPrice) {
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

void Order::setQuantity(int quantity) {
    quantity_ = quantity;
};
