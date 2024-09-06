#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <chrono>

class Order {
public:
    // Enum for order side (buy or sell)
    enum class Side {
        BUY,
        SELL
    };

    // Constructors
    Order() = default;
    Order(int userOrderId, int userId, const std::string& symbol, double price, int quantity, Side side);

    // Getters
    int getUserOrderId() const;
    int getUserId() const;
    std::string getSymbol() const;
    double getPrice() const;
    int getQuantity() const;
    Side getSide() const;
    std::chrono::system_clock::time_point getTimestamp() const;

    // Setters
    void setUserOrderId(int userOrderId);
    void setUserId(int userId);
    void setSymbol(const std::string& symbol);
    void setPrice(double price);
    void setQuantity(int quantity);
    void setSide(Side side);
    void setTimestamp(const std::chrono::system_clock::time_point& timestamp);

    // Utility function to convert Side enum to string
    static std::string sideToString(Side side);

private:
    int userOrderId;
    int userId;
    std::string symbol;
    double price;
    int quantity;
    Side side;
    std::chrono::system_clock::time_point timestamp;
};

#endif // ORDER_H
