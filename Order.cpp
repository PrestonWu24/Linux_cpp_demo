#include "Order.h"

// Default constructor
Order::Order(int userOrderId, int userId, const std::string& symbol, double price, int quantity, Side side)
    : userOrderId(userOrderId), userId(userId), symbol(symbol), price(price), quantity(quantity), side(side), timestamp(std::chrono::system_clock::now()) {}

// Getters
int Order::getUserOrderId() const { return userOrderId; }
int Order::getUserId() const { return userId; }
std::string Order::getSymbol() const { return symbol; }
double Order::getPrice() const { return price; }
int Order::getQuantity() const { return quantity; }
Order::Side Order::getSide() const { return side; }
std::chrono::system_clock::time_point Order::getTimestamp() const { return timestamp; }

// Setters
void Order::setUserOrderId(int userOrderId) { this->userOrderId = userOrderId; }
void Order::setUserId(int userId) { this->userId = userId; }
void Order::setSymbol(const std::string& symbol) { this->symbol = symbol; }
void Order::setPrice(double price) { this->price = price; }
void Order::setQuantity(int quantity) { this->quantity = quantity; }
void Order::setSide(Side side) { this->side = side; }
void Order::setTimestamp(const std::chrono::system_clock::time_point& timestamp) { this->timestamp = timestamp; }

// Utility function to convert Side enum to string
std::string Order::sideToString(Side side) {
    switch (side) {
        case Side::BUY:
            return "BUY";
        case Side::SELL:
            return "SELL";
        default:
            return "UNKNOWN";
    }
}
