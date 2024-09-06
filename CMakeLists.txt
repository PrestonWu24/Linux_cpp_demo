cmake_minimum_required(VERSION 3.22.1)
project(OrderBookSrv)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Boost (specify components if using specific libraries)
find_package(Boost 1.74 REQUIRED COMPONENTS system filesystem)

# Include directories for Boost
include_directories(${Boost_INCLUDE_DIRS})

# List your source files
set(SOURCES
    app/main.cpp
    app/Order.cpp
    app/Order.h
    app/OrderBook.cpp
    app/OrderBook.h
    app/OrderMatchEngine.cpp
    app/OrderMatchEngine.h
    app/OrderServer.cpp
    app/OrderServer.h
    app/MessageQueue.cpp
    app/MessageQueue.h
    app/MessageParser.cpp
    app/MessageParser.h
)

# Include Google Test
add_subdirectory(googletest)

# Test sources
set(TEST_SOURCES
    app/Order.cpp
    app/Order.h
    app/OrderBook.cpp
    app/OrderBook.h
    app/OrderMatchEngine.cpp
    app/OrderMatchEngine.h
    app/OrderServer.cpp
    app/OrderServer.h
    app/MessageQueue.cpp
    app/MessageQueue.h
    app/MessageParser.cpp
    app/MessageParser.h
    test/test.cpp  # Include test.cpp here
)

# Define the test executable
add_executable(OrderBookSrvTest ${TEST_SOURCES})

# Define the executable target
add_executable(OrderBookSrv ${SOURCES})


# Link Boost libraries (only link Boost components you specified)
target_link_libraries(OrderBookSrv Boost::system Boost::filesystem)

# Link Boost libraries to the test executable
target_link_libraries(OrderBookSrvTest gtest gtest_main Boost::system Boost::filesystem)

# Include directories for test
target_include_directories(OrderBookSrvTest PRIVATE ${Boost_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/app)

# Enable testing and add a test
enable_testing()
add_test(NAME OrderBookSrvTest COMMAND OrderBookSrvTest)

