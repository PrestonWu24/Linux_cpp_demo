#include "OrderMatchEngine.h"
#include <iostream>
#include <mutex>

OrderMatchEngine::OrderMatchEngine() {}

OrderMatchEngine::~OrderMatchEngine() {}

void OrderMatchEngine::addOrder(const Order& order) {
    std::unique_lock lock(orderBooksMutex);
    auto& orderBook = getOrderBook(order.getSymbol());
    matchOrder(*orderBook, order);
}

bool OrderMatchEngine::cancelOrder(int userOrderId, int userId) {
    std::shared_lock lock(orderBooksMutex);
    for (auto& [symbol, orderBook] : orderBooks) {
        if (orderBook->cancelOrder(userOrderId, userId)) {
            // Potentially publish a top-of-book change here
            auto bestBid = orderBook->getBestBid();
            auto bestAsk = orderBook->getBestAsk();
            publishCancel(userOrderId, userId);
            publishTopOfBookChange(symbol, bestBid, bestAsk);
            return true;
        }
    }
    return false;
}

void OrderMatchEngine::flush() {
    std::shared_lock lock(orderBooksMutex);
    for (auto& [symbol, orderBook] : orderBooks) {
        orderBook->flush();
    }
}

std::pair<std::shared_ptr<Order>, std::shared_ptr<Order>> OrderMatchEngine::getTopOfBook(const std::string& symbol) const {
    std::shared_lock lock(orderBooksMutex);
    const auto it = orderBooks.find(symbol);
    if (it != orderBooks.end()) {
        return { it->second->getBestBid(), it->second->getBestAsk() };
    }
    return { nullptr, nullptr };
}

std::pair<const std::multimap<double, std::shared_ptr<Order>>&, const std::multimap<double, std::shared_ptr<Order>>&> OrderMatchEngine::getOrders(const std::string& symbol) const {
    std::shared_lock lock(orderBooksMutex);
    static const std::multimap<double, std::shared_ptr<Order>> emptyMap;
    const auto it = orderBooks.find(symbol);
    if (it != orderBooks.end()) {
        return { it->second->getBuyOrders(), it->second->getSellOrders() };
    }
    return { emptyMap, emptyMap };
}

std::unique_ptr<OrderBook>& OrderMatchEngine::getOrderBook(const std::string& symbol) {
    auto it = orderBooks.find(symbol);
    if (it == orderBooks.end()) {
        auto newOrderBook = std::make_unique<OrderBook>(symbol);
        it = orderBooks.emplace(symbol, std::move(newOrderBook)).first;
    }
    return it->second;
}

void OrderMatchEngine::matchOrder(OrderBook& book, const Order& incomingOrder) {
    if (incomingOrder.getSide() == Order::Side::BUY) {
        while (auto bestAsk = book.getBestAsk()) {
            if (incomingOrder.getPrice() >= bestAsk->getPrice()) {
                publishTrade(*bestAsk, incomingOrder);
                // Remove the matched ask order
                book.cancelOrder(bestAsk->getUserOrderId(), bestAsk->getUserId());
                if (incomingOrder.getQuantity() == 0) break;
            } else {
                break;
            }
        }
        if (incomingOrder.getQuantity() > 0) {
            book.addOrder(incomingOrder);
            publishAck(incomingOrder);
        }
    } else {
        while (auto bestBid = book.getBestBid()) {
            if (incomingOrder.getPrice() <= bestBid->getPrice()) {
                publishTrade(*bestBid, incomingOrder);
                // Remove the matched bid order
                book.cancelOrder(bestBid->getUserOrderId(), bestBid->getUserId());
                if (incomingOrder.getQuantity() == 0) break;
            } else {
                break;
            }
        }
        if (incomingOrder.getQuantity() > 0) {
            book.addOrder(incomingOrder);
            publishAck(incomingOrder);
        }
    }

    // After matching, publish the top of the book change
    auto bestBid = book.getBestBid();
    auto bestAsk = book.getBestAsk();
    publishTopOfBookChange(book.getSymbol(), bestBid, bestAsk);
}

void OrderMatchEngine::publishAck(const Order& order) {
    // Implementation for publishing acknowledgements (e.g., to a message queue)
    std::cout << "Acknowledgement: Order added: userId = " << order.getUserId() 
              << " and user orderid = "<< order.getUserOrderId() << "\n";
}

void OrderMatchEngine::publishCancel(int userOrderId, int userId) {
    // Implementation for publishing acknowledgements (e.g., to a message queue)
    std::cout << "Cancel Order added: userId = " << userId 
              << " and user orderid = "<< userOrderId << "\n";
}

void OrderMatchEngine::publishTrade(const Order& matchedOrder, const Order& incomingOrder) {
    // Implementation for publishing trades (e.g., to a trading feed)
    std::cout << "Trade: " << matchedOrder.getQuantity() << " units at " << matchedOrder.getPrice()
              << " between userId = " << matchedOrder.getUserId() <<" user orderId = " << matchedOrder.getUserOrderId() 
              << " and userId = " << incomingOrder.getUserId() <<" user orderId = " << incomingOrder.getUserOrderId()<< "\n";
}

void OrderMatchEngine::publishTopOfBookChange(const std::string& symbol, const std::shared_ptr<Order>& bestBid, const std::shared_ptr<Order>& bestAsk) {
    // Implementation for publishing top-of-book changes (e.g., to a market data feed)
    std::cout << "Top of Book Change for " << symbol << ": "
              << "Best Bid = " << (bestBid ? std::to_string(bestBid->getPrice()) : "None") << ", "
              << "Best Bid qty = " << (bestBid ? std::to_string(bestBid->getQuantity()) : "None") << ", "
              << "Best Ask = " << (bestAsk ? std::to_string(bestAsk->getPrice()) : "None") << ", "
              << "Best Ask qty= " << (bestAsk ? std::to_string(bestAsk->getQuantity()) : "None") << "\n";
}
