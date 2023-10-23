#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <grpcpp/grpcpp.h>
#include "helloworld.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

zmq::context_t context{1};
zmq::socket_t publisher{context, zmq::socket_type::pub};
zmq::socket_t subscriber{context, zmq::socket_type::sub};

class GreeterServiceImpl final : public Greeter::Service
{
public:
    Status SayHello(ServerContext *context, const HelloRequest *request, HelloReply *reply) override
    {
        std::string prefix("Hello, ");
        reply->set_message(prefix + request->name());
        publisher.send(zmq::str_buffer("Hello"), zmq::send_flags::sndmore);
        publisher.send(zmq::buffer(request->name()));
        std::cout << "Sent: " << request->name() << std::endl;
        return Status::OK;
    }
};

void RunPublisher()
{   
    // bind all ip
    publisher.bind("tcp://*:5555");
}

void RunSubscriber()
{   
    // another service zeromq ip address 
    subscriber.connect("tcp://192.168.2.64:5001");
    subscriber.set(zmq::sockopt::subscribe, "Hello");

    if (subscriber.connected()){
        std::cout << "Subscriber listen!" << std::endl;
    }

    while (true)
    {
        std::vector<zmq::message_t> recv_msgs;
        zmq::recv_result_t result = zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
        assert(result && "recv failed");
        assert(*result == 2);

        std::cout << "Subscriber: [" << recv_msgs[0].to_string() << "] " << recv_msgs[1].to_string() << std::endl;
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}

void RunServer()
{
    std::cout << "hello world" << std::endl;

    try
    {   
        // self grpc ip
        std::string server_address("192.168.2.173:5000");
        GreeterServiceImpl service;

        ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);

        std::unique_ptr<Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << server_address << std::endl;

        // Start publisher and subscriber threads
        std::thread publisherThread(RunPublisher);
        std::thread subscriberThread(RunSubscriber);

        server->Wait();

        // Wait for publisher and subscriber threads to finish
        publisherThread.join();
        subscriberThread.join();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}

int main()
{
    RunServer();
    return 0;
}
