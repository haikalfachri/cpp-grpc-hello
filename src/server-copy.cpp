#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <grpcpp/grpcpp.h>
#include "helloworld.grpc.pb.h"
#include "crow.h"

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

std::string message = "woi";

void RunSSEServer()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/sse").methods("GET"_method)([&](crow::response &res)
                                                  {
        // Set the content type to SSE
        

        // Create a new subscriber socket for SSE
        // zmq::socket_t sse_subscriber{context, zmq::socket_type::sub};
        // sse_subscriber.connect("tcp://192.168.2.64:5001");
        // sse_subscriber.set(zmq::sockopt::subscribe, "Hello");

        // if (sse_subscriber.connected()) {
        //     std::cout << "SSE Subscriber listening!" << std::endl;
        // }

        // std::vector<zmq::message_t> recv_msgs;
        // zmq::recv_result_t result = zmq::recv_multipart(sse_subscriber, std::back_inserter(recv_msgs));
        // assert(result && "recv failed");
        // assert(*result == 2);

        // Prepare SSE message and send it to the client
        do{
            res.add_header("Content-Type", "text/event-stream");
            res.add_header("Cache-Control", "no-cache");
            res.set_header("Connection", "keep-alive");
            res.add_header("Content-Length", std::to_string(message.size()));
            // res.set_header("Content-Length", std::to_string(message.size()));
            // message = "";
            // res.is_completed
            res.end("data: " + message + "\n\n");
            res.clear();
            std::cout << std::to_string(res.is_completed()) << std::endl;
        } while(!res.is_completed()); });

    app.port(8080).multithreaded().run();
}

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
    publisher.bind("tcp://localhost:5555");
}

void RunSubscriber()
{
    subscriber.connect("tcp://localhost:5555");
    subscriber.set(zmq::sockopt::subscribe, "Hello");

    if (subscriber.connected())
    {
        std::cout << "Subscriber listen!" << std::endl;
    }

    while (true)
    {
        std::vector<zmq::message_t> recv_msgs;
        zmq::recv_result_t result = zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
        assert(result && "recv failed");
        assert(*result == 2);
        message = "[" + recv_msgs[0].to_string() + "] " + recv_msgs[1].to_string();
        std::cout << "Subscriber: " << message << std::endl;
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}

void RunServer()
{
    std::cout << "hello world" << std::endl;

    try
    {
        std::string server_address("localhost:5000");
        GreeterServiceImpl service;

        ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);

        std::unique_ptr<Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << server_address << std::endl;

        std::thread publisherThread(RunPublisher);
        std::thread subscriberThread(RunSubscriber);
        std::thread sseServerThread(RunSSEServer); // Add SSE server thread

        server->Wait();
        publisherThread.join();
        subscriberThread.join();
        sseServerThread.join(); // Wait for SSE server thread to finish
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
