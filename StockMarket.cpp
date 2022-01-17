#include "StockMarket.h"
#include <iostream>
#include <string>
// ================== Limit Order Class ==================

LimitOrder::LimitOrder(string orderID, char action, char type, char div, float limitPrice, int quantity){
    orderID_ = orderID;
    action_ = action;
    type_ = type;
    div_ = div;
    limitPrice_ = limitPrice;
    quantity_ = quantity;
}

test::test(string text) {
    message = text;
}
