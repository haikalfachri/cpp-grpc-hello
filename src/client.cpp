#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "helloworld.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloRequest;
using helloworld::HelloReply;

class GreeterClient {
public:
    GreeterClient(std::shared_ptr<Channel> channel)
        : stub_(Greeter::NewStub(channel)) {}

    std::string SayHello(const std::string& name) {
        HelloRequest request;
        HelloReply reply;
        ClientContext context;

        request.set_name(name);

        Status status = stub_->SayHello(&context, request, &reply);

        if (status.ok()) {
            return reply.message();
        } else {
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<Greeter::Stub> stub_;
};

int main() {
    std::string server_address("localhost:5000");
    while (true) {
        try {
            GreeterClient greeter(grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials()));
            std::string user("World");
            std::string response = greeter.SayHello(user);
            std::cout << "Greeting: " << response << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Exception caught: " << e.what() << std::endl;
        }
    }
   
    return 0;
}
