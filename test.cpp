#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include "OrderBook.h"          // Include relevant headers
#include "MessageQueue.h"
#include "MessageParser.h"
#include "OrderMatchEngine.h"


class OrderServerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize components needed for the test
        messageQueue = std::make_shared<MessageQueue>();
        orderMatchEngine = std::make_unique<OrderMatchEngine>();
        messageParser = std::make_unique<MessageParser>(messageQueue, std::move(orderMatchEngine));
    }

    std::shared_ptr<MessageQueue> messageQueue;
    std::unique_ptr<MessageParser> messageParser;
    std::unique_ptr<OrderMatchEngine> orderMatchEngine;
};

// Test that reads from inputFile.csv and processes the messages
TEST_F(OrderServerTest, OrderBookTest) {
    // Read from inputFile.csv
    std::ifstream file("inputFile.csv");
    ASSERT_TRUE(file.is_open()) << "Failed to open input file";

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line[0] != '#') {  // Skip comments
            messageQueue->enqueue(line);
        }
    }

    file.close();

    // Process messages with the parser
    messageParser->start();
    // Stop the parser after processing
    messageParser->stop();
}

