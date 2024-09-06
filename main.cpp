#include <iostream>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/bind/bind.hpp>
#include <csignal>
#include <memory>
#include "OrderServer.h"

// Signal handler for graceful shutdown
std::unique_ptr<boost::asio::io_context> io_context;
std::unique_ptr<OrderServer> order_server;

void handleSignal(const boost::system::error_code& error, int signal_number) {
    if (!error) {
        std::cout << "Received signal " << signal_number << ", stopping server..." << std::endl;
        if (order_server) {
            order_server->stop();
        }
        if (io_context) {
            io_context->stop();
        }
        exit(signal_number);  // Immediate exit, ensure all resources are released
    } else {
        std::cerr << "Error in signal handling: " << error.message() << std::endl;
    }
}

int main() {
    try {
        // Initialize the io_context and server
        io_context = std::make_unique<boost::asio::io_context>();
        // Create a UDP endpoint for the specified IP address and port
        boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::make_address("127.0.0.1"), 1234);

        order_server = std::make_unique<OrderServer>(*io_context, endpoint);

        // Start the order server
        order_server->start();
        std::cout << "Order book server is running on 127.0.0.1:1234" << std::endl;

        // Signal set to handle SIGINT and SIGTERM for graceful shutdown
        boost::asio::signal_set signals(*io_context, SIGINT, SIGTERM);
        signals.async_wait(boost::bind(&handleSignal, boost::asio::placeholders::error, boost::asio::placeholders::signal_number));

        // Run the io_context to start listening for messages
        io_context->run();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    std::cout << "Server stopped." << std::endl;
    return 0;
}
