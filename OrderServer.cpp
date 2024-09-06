#include "OrderServer.h"
#include <iostream>
#include <boost/bind/bind.hpp>

OrderServer::OrderServer(boost::asio::io_context& io_context, boost::asio::ip::udp::endpoint endpoint)
    : socket_(io_context, endpoint),
      messageQueue_(std::make_shared<MessageQueue>()) {
}

OrderServer::~OrderServer() {
    stop();
}

void OrderServer::start() {
    messageParser_ = std::make_unique<MessageParser>(messageQueue_, std::make_shared<OrderMatchEngine>());
    messageParser_->start();

    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_), remote_endpoint_,
        boost::bind(&OrderServer::handleReceive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void OrderServer::stop() {
    socket_.close();
    messageParser_->stop();
}

void OrderServer::handleReceive(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (!error && bytes_transferred > 0) {
        std::string message(recv_buffer_.data(), bytes_transferred);
        messageQueue_->enqueue(message);
    }
    else{
        std::cerr << "Error in receive: " << error.message() << std::endl;
    }

    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_), remote_endpoint_,
        boost::bind(&OrderServer::handleReceive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}
