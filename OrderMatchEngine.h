#ifndef ORDERMATCHENGINE_H
#define ORDERMATCHENGINE_H

#include <unordered_map>
#include <memory>
#include <shared_mutex>
#include "OrderBook.h"
#include "Order.h"

class OrderMatchEngine {
public:
    OrderMatchEngine();
    ~OrderMatchEngine();

    // Add a new order to the appropriate order book
    void addOrder(const Order& order);

    // Cancel an existing order
    bool cancelOrder(int userOrderId, int userId);

    void flush();

    // Get the best bid and ask for a given symbol
    std::pair<std::shared_ptr<Order>, std::shared_ptr<Order>> getTopOfBook(const std::string& symbol) const;

    // Get all orders for a given symbol (for testing or reporting purposes)
    std::pair<const std::multimap<double, std::shared_ptr<Order>>&, const std::multimap<double, std::shared_ptr<Order>>&> getOrders(const std::string& symbol) const;

private:
    // Map of symbols to their corresponding order books
    std::unordered_map<std::string, std::unique_ptr<OrderBook>> orderBooks;

    // Mutex for concurrent access
    mutable std::shared_mutex orderBooksMutex;

    // Find or create the order book for a given symbol
    std::unique_ptr<OrderBook>& getOrderBook(const std::string& symbol);
    
    // Match an incoming order with existing orders in the order book
    void matchOrder(OrderBook& book, const Order& incomingOrder);
    
    // Publish acknowledgements, trades, and top-of-book changes
    void publishAck(const Order& order);
    void publishCancel(int userOrderId, int userId);
    void publishTrade(const Order& matchedOrder, const Order& incomingOrder);
    void publishTopOfBookChange(const std::string& symbol, const std::shared_ptr<Order>& bestBid, const std::shared_ptr<Order>& bestAsk);
};

#endif // ORDERMATCHENGINE_H
