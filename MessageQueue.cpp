#include "MessageQueue.h"

void MessageQueue::enqueue(const std::string& message) {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(message);
    condVar_.notify_one();
}

std::string MessageQueue::dequeue() {
    std::unique_lock<std::mutex> lock(mutex_);
    condVar_.wait(lock, [this] { return !queue_.empty(); });
    std::string message = queue_.front();
    queue_.pop();
    return message;
}
