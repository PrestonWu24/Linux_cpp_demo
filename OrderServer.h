#ifndef ORDERSERVER_H
#define ORDERSERVER_H

#include <boost/asio.hpp>
#include <thread>
#include <memory>
#include "MessageQueue.h"
#include "MessageParser.h"

class OrderServer {
public:
    OrderServer(boost::asio::io_context& io_context, boost::asio::ip::udp::endpoint endpoint);
    ~OrderServer();

    void start();
    void stop();

private:
    void handleReceive(const boost::system::error_code& error, std::size_t bytes_transferred);
    void processMessage(const std::string& message);

    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint remote_endpoint_;
    std::array<char, 10240> recv_buffer_;
    std::shared_ptr<MessageQueue> messageQueue_;
    std::unique_ptr<MessageParser> messageParser_;
};

#endif // ORDERSERVER_H
