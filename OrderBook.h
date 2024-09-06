#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <map>
#include <unordered_map>
#include <memory>
#include <vector>
#include "Order.h"

class OrderBook {
public:
    OrderBook(const std::string& symbol);
    ~OrderBook();

    // Add a new order
    void addOrder(const Order& order);

    // Cancel an existing order
    bool cancelOrder(int userOrderId, int userId);

    // Flush order book
    void flush();

    // Get the top of the book for bids and asks
    std::string getSymbol() const;
    std::shared_ptr<Order> getBestBid() const;
    std::shared_ptr<Order> getBestAsk() const;

    // Get all bids and asks
    const std::multimap<double, std::shared_ptr<Order>>& getBuyOrders() const;
    const std::multimap<double, std::shared_ptr<Order>>& getSellOrders() const;

private:
    std::string symbol;

    // Multimaps for storing orders, sorted by price
    std::multimap<double, std::shared_ptr<Order>> buyOrders; // For bids
    std::multimap<double, std::shared_ptr<Order>> sellOrders; // For asks

    // A map to track orders by userOrderId for fast cancellation
    std::unordered_map<int, std::shared_ptr<Order>> orderIdMap;
};

#endif // ORDERBOOK_H
