#include "OrderBook.h"


OrderBook::OrderBook(const std::string& symbol) : symbol(symbol) {}

OrderBook::~OrderBook() {}

void OrderBook::addOrder(const Order& order) {
    std::shared_ptr<Order> newOrder = std::make_shared<Order>(order);

    if (order.getSide() == Order::Side::BUY) {
        buyOrders.insert({order.getPrice(), newOrder});
    } else {
        sellOrders.insert({order.getPrice(), newOrder});
    }

    orderIdMap[order.getUserOrderId()] = newOrder;
}

bool OrderBook::cancelOrder(int userOrderId, int userId) {
    auto it = orderIdMap.find(userOrderId);
    if (it != orderIdMap.end()) {
        auto& order = it->second;

        if (order->getSide() == Order::Side::BUY) {
            auto range = buyOrders.equal_range(order->getPrice());
            for (auto itBuy = range.first; itBuy != range.second; ++itBuy) {
                if (itBuy->second->getUserOrderId() == userOrderId) {
                    buyOrders.erase(itBuy);
                    break;
                }
            }
        } else {
            auto range = sellOrders.equal_range(order->getPrice());
            for (auto itSell = range.first; itSell != range.second; ++itSell) {
                if (itSell->second->getUserOrderId() == userOrderId) {
                    sellOrders.erase(itSell);
                    break;
                }
            }
        }

        orderIdMap.erase(it);
        return true;
    }

    return false;
}

// Flush the order book by clearing all orders
void OrderBook::flush() {
    buyOrders.clear();
    sellOrders.clear();
    orderIdMap.clear();
}

std::string OrderBook::getSymbol() const { return symbol; }

std::shared_ptr<Order> OrderBook::getBestBid() const {
    if (buyOrders.empty()) return nullptr;
     return buyOrders.rbegin()->second;
}

std::shared_ptr<Order> OrderBook::getBestAsk() const {
    if (sellOrders.empty()) return nullptr;
    return sellOrders.begin()->second;
}

const std::multimap<double, std::shared_ptr<Order>>& OrderBook::getBuyOrders() const {
    return buyOrders;
}

const std::multimap<double, std::shared_ptr<Order>>& OrderBook::getSellOrders() const {
    return sellOrders;
}
