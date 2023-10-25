#include <iostream>
#include <string>
#include <unordered_map>
#include <chrono>
#include <grpcpp/grpcpp.h>
#include "user.pb.h"
#include "user.grpc.pb.h"

using google::protobuf::Empty;
using google::protobuf::Int64Value;
using google::protobuf::Timestamp;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using user::User;
using user::UserService;
using namespace std;

class UserServiceImpl final : public UserService::Service
{
public:
    // UserServiceImpl() {
    //     // Inisialisasi data pengguna (biasanya ini akan berasal dari database)
    //     users_[1] = {1, "John Doe", {}, {}};
    //     users_[2] = {2, "Jane Smith", {}, {}};
    // }

    Status CreateUser(ServerContext *context, const User *request, User *response) override
    {
        int64_t newUserId = GenerateNewUserId(); // Implement your logic to generate a new user ID
        User newUser;
        newUser.set_id(newUserId);
        newUser.set_name(request->name());
        // Set created_at and updated_at fields as per time now
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        // Set created_at to the current time
        Timestamp *createdTimestamp = new Timestamp;
        createdTimestamp->set_seconds(std::chrono::system_clock::now().time_since_epoch().count());
        createdTimestamp->set_nanos(0);
        newUser.set_allocated_created_at(createdTimestamp);

        // Set updated_at to the current time
        Timestamp *updatedTimestamp = new Timestamp;
        updatedTimestamp->set_seconds(std::chrono::system_clock::now().time_since_epoch().count());
        updatedTimestamp->set_nanos(0);
        newUser.set_allocated_updated_at(updatedTimestamp);

        // Konversi created_at ke string dan cetak
        std::tm tm_struct = *std::localtime(&now);
        char buffer[20]; // Buffer for formatted time string, assuming maximum size
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm_struct);

        std::cout << "Timestamp: " << buffer << std::endl;
        
        users_[newUserId] = newUser;
        *response = newUser;
        return Status::OK;
    }

    Status ReadUser(ServerContext *context, const Int64Value *request, User *response) override
    {
        int64_t userId = request->value();
        auto it = users_.find(userId);
        if (it != users_.end())
        {
            *response = it->second;
            return Status::OK;
        }
        else
        {
            return Status(grpc::StatusCode::NOT_FOUND, "User not found");
        }
    }

    Status UpdateUser(ServerContext *context, const User *request, User *response) override
    {
        int64_t userId = request->id();
        auto it = users_.find(userId);
        if (it != users_.end())
        {
            User &existingUser = it->second;
            // Update the user fields as needed
            existingUser.set_name(request->name());
            // Update the updated_at field
            *response = existingUser;
            return Status::OK;
        }
        else
        {
            return Status(grpc::StatusCode::NOT_FOUND, "User not found");
        }
    }

    Status DeleteUser(ServerContext *context, const Int64Value *request, Empty *response) override
    {
        int64_t userId = request->value();
        auto it = users_.find(userId);
        if (it != users_.end())
        {
            users_.erase(it);
            return Status::OK;
        }
        else
        {
            return Status(grpc::StatusCode::NOT_FOUND, "User not found");
        }
    }

    Status ListUsers(ServerContext *context, const Empty *request, grpc::ServerWriter<User> *writer) override
    {
        for (const auto &pair : users_)
        {
            writer->Write(pair.second);
        }
        return Status::OK;
    }

private:
    int64_t GenerateNewUserId()
    {
        // Implement your logic to generate a new unique user ID
        // This is just a placeholder
        static int64_t nextId = 3;
        return nextId++;
    }

    std::unordered_map<int64_t, User> users_;
};

void RunServer()
{
    std::string server_address("localhost:5002");
    UserServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main(int argc, char **argv)
{
    RunServer();
    return 0;
}
