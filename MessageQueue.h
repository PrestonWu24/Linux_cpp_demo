#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>

class MessageQueue {
public:
    void enqueue(const std::string& message);
    std::string dequeue();

private:
    std::queue<std::string> queue_;
    std::mutex mutex_;
    std::condition_variable condVar_;
};

#endif // MESSAGEQUEUE_H
