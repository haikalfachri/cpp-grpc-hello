#pragma once

#include <grpcpp/grpcpp.h>
#include <chrono>
#include <iostream>
#include <pqxx/pqxx>
#include <nlohmann/json.hpp>
#include "rapidjson/document.h"
#include "../generated_protos/user.grpc.pb.h"
#include "../generated_protos/user.pb.h"
#include "../globals/globals.h"

class UserServiceImpl final : public user::UserService::Service {
public:
    grpc::Status CreateUser(grpc::ServerContext *context, const user::User *request, user::User *response) override;
    grpc::Status ReadUser(grpc::ServerContext *context, const google::protobuf::Int64Value *request, user::User *response) override;
    grpc::Status UpdateUser(grpc::ServerContext *context, const user::User *request, user::User *response) override;
    grpc::Status DeleteUser(grpc::ServerContext *context, const google::protobuf::Int64Value *request, user::User *response) override;
    grpc::Status ListUsers(grpc::ServerContext *context, const google::protobuf::Empty *request, grpc::ServerWriter<user::User> *writer) override;
};
