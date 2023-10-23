//  Send a message on the 'Hello' topic
#include <string>
#include <iostream>
#include <conio.h>
#include <thread>

#include <zmq.hpp>

int main()
{
    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // construct a PUB (publisher) socket and connect to interface
    zmq::socket_t publisher{context, zmq::socket_type::pub};
    publisher.bind("tcp://localhost:5001");

    int i = 0;
    while (!_kbhit()) {
        // Create a stringstream to build the message
        std::stringstream ss;
        ss << "Hello " << i;

        // Convert the stringstream to a string
        std::string message = ss.str();

        // Send the message
        publisher.send(zmq::str_buffer("Hello"), zmq::send_flags::sndmore);
        publisher.send(zmq::buffer(message));
        std::cout << "[Hello] " << message << std::endl;
        i++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    while (true)
    {
        std::cout << "Not send!" << std::endl;
    }

    return 0;
}
