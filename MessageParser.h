#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include <string>
#include <vector>
#include <memory>
#include <thread>
#include "MessageQueue.h"
#include "OrderMatchEngine.h"


class MessageParser {
public:
    MessageParser(std::shared_ptr<MessageQueue> queue, std::shared_ptr<OrderMatchEngine> engine);
    void start();
    void stop();

private:
    std::shared_ptr<MessageQueue> messageQueue_;
    std::shared_ptr<OrderMatchEngine> matchEngine_;
    bool running_;
    std::thread workerThread_;
    std::string remainder_;  

    void processMessages();
    void parseMessage(const std::string& message);
    std::vector<std::string> split(const std::string& str, char delimiter);
    std::string_view ltrim(std::string_view sv);
    std::string_view rtrim(std::string_view sv);
    std::string_view trim(std::string_view sv);
};

#endif // MESSAGEPARSER_H
