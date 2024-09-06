Components of the OrderBook Server
OrderServer: Listens on 127.0.0.1:1234 for incoming connections.
OrderBook: Manages the order book, including adding, removing, and querying orders.
Order: Represents an order with details like user ID, order ID, symbol, price, quantity, side (buy/sell), and timestamp.
MessageQueue: Queues all incoming messages for processing.
MessageParser: Parses incoming messages into structured data.
The packages include all source file h and cpp files, Dockerfile to build docker image, CMakeLists.txt to compile and link the executives, inputFile.csv to test, nohup.out the result for my test.


Building the Docker Image
To build the Docker image with all necessary packages, use:
docker build -t kraken_image .
Running the Docker Container
To run the Docker container and mount your local directory to /usr/src/app inside the container:

docker run -v /path/to/local/dir:/usr/src/app -it kraken_image
Replace /path/to/local/dir with your local directory path.

Compiling and Linking the Project
To compile and link the project:

pwd # it should be /usr/src/app
cd ..
mkdir build
mkdir test
cp app/test.cpp test
cp app/CMakeLists.txt .
cp app/inputFile.csv build
cd build
Run CMake and make:
cmake ..
make
This will produce the OrderBookSrv and OrderBookSrvTest executable.

The file structure should be as follows:
root@fa262d2487a4:/usr/src# ls
CMakeLists.txt  app  build  googletest  gtest  test

Running the Server
To start the server:
nohup ./OrderBookSrv &
The server will run in the background, and output will be written to nohup.txt.

Sending Test Data
To send test data to the server:
cat inputFile.csv | netcat -u 127.0.0.1 1234
This sends the contents of inputFile.csv to the server using UDP.

Collecting Output
The server's output can be found in the nohup.out file. The output is one line of input with server response for best reference

Running the test
nohup ./OrderBookSrvTest &
currently the test read the whole inputFile and process as the server, it should break into smaller tests and verify the results







