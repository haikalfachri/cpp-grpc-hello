#pragma once

#include <grpcpp/grpcpp.h>
#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
#include <pqxx/pqxx>
#include <nlohmann/json.hpp>
#include "rapidjson/document.h"
#include "../generated_protos/user.grpc.pb.h"
#include "../generated_protos/user.pb.h"
#include "../globals/globals.h"

using user::User;
using user::UserService;
using google::protobuf::Empty;
using google::protobuf::Int64Value;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using namespace std;

class UserServiceImpl final : public UserService::Service {
public:
    Status CreateUser(ServerContext *context, const User *request, User *response) override;
    Status ReadUser(ServerContext *context, const Int64Value *request, User *response) override;
    Status UpdateUser(ServerContext *context, const User *request, User *response) override;
    Status DeleteUser(ServerContext *context, const Int64Value *request, User *response) override;
    Status ListUsers(ServerContext *context, const Empty *request, grpc::ServerWriter<User> *writer) override;
};
