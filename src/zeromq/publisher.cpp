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
    publisher.bind("tcp://*:5001");

    int i = 0;
    while (!_kbhit()) {
        //  Write three messages, each with an envelope and content
        publisher.send(zmq::str_buffer("Hello"), zmq::send_flags::sndmore);
        publisher.send(zmq::str_buffer("Hello!"));
        std::cout << "[Hello] Hello!" << std::endl;
        i++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    while (true) {
        std::cout << "Not send!" << std::endl;
    }

    return 0;
}
