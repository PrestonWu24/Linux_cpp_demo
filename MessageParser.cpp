#include "MessageParser.h"
#include <sstream>
#include <iostream>



MessageParser::MessageParser(std::shared_ptr<MessageQueue> queue, std::shared_ptr<OrderMatchEngine> engine)
    : messageQueue_(queue), matchEngine_(engine), running_(false) {}

void MessageParser::start() {
    running_ = true;
    workerThread_ = std::thread(&MessageParser::processMessages, this);
}

void MessageParser::stop() {
    running_ = false;
    if (workerThread_.joinable()) {
        workerThread_.join();
    }
}

void MessageParser::processMessages() {
    while (running_) {
        std::string message = messageQueue_->dequeue();
        std::string completeMessage = remainder_ + message;
        remainder_.clear();
        std::istringstream stream(completeMessage);
        std::string line;

        while (std::getline(stream, line)) {
            // Skip empty lines
            if (line.empty()) continue;
            // Process the line using the provided function
            if (stream.eof() && completeMessage.back() != '\n') {
                remainder_ = line;
            } 
            else
            {
                std::cout << line << std::endl;
                line = trim(line);
                parseMessage(line);
            }

        }
  
    }
}

void MessageParser::parseMessage(const std::string& message) {
    // Skip comment lines
    if (message.empty() || message[0] == '#') {
        return;
    }

    std::vector<std::string> tokens = split(message, ',');
    if (tokens.empty()) {
        return;
    }

    const std::string& command = tokens[0];
    if (command == "N") {
        if (tokens.size() < 7) {
            std::cerr << "Invalid new order format\n";
            return;
        }
        try {
            int userId = std::stoi(tokens[1]);
            std::string symbol = tokens[2];
            double price = std::stod(tokens[3]);
            int quantity = std::stoi(tokens[4]);
            Order::Side side = (tokens[5] == "B") ? Order::Side::BUY : Order::Side::SELL;
            int userOrderId = std::stoi(tokens[6]);
            Order newOrder(userOrderId, userId, symbol, price, quantity, side);
            matchEngine_->addOrder(newOrder);
        } catch (const std::exception& e) {
            std::cerr << "Error processing new order: " << e.what() << "\n";
        }
    } else if (command == "C") {
        if (tokens.size() < 3) {
            std::cerr << "Invalid cancel order format\n";
            return;
        }
        try {
            int userId = std::stoi(tokens[1]);
            int userOrderId = std::stoi(tokens[2]);
            matchEngine_->cancelOrder(userOrderId, userId);
        } catch (const std::exception& e) {
            std::cerr << "Error processing cancel order: " << e.what() << "\n";
        }
    } else if (command == "F") {
        try {
            matchEngine_->flush();
        } catch (const std::exception& e) {
            std::cerr << "Error processing flush order book: " << e.what() << "\n";
        }
    } else {
        std::cerr << "Unknown command: " << command << "\n";
    }
}

std::vector<std::string> MessageParser::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        token = trim(token);
        tokens.push_back(token);
    }
    return tokens;
}

// Trim from start
std::string_view MessageParser::ltrim(std::string_view sv) {
    sv.remove_prefix(std::min(sv.find_first_not_of(" \t\n\r\f\v"), sv.size()));
    return sv;
}

// Trim from end
std::string_view MessageParser::rtrim(std::string_view sv) {
    sv.remove_suffix(std::min(sv.size() - sv.find_last_not_of(" \t\n\r\f\v") - 1, sv.size()));
    return sv;
}

// Trim from both ends
std::string_view MessageParser::trim(std::string_view sv) {
    return ltrim(rtrim(sv));
}
