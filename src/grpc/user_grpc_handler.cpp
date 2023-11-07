#include "user_grpc_handler.h"
#include "../globals/globals.h"

using user::User;
using user::UserService;
using google::protobuf::Empty;
using google::protobuf::Int64Value;
using grpc::Status;
using namespace std;

grpc::Status UserServiceImpl::CreateUser(grpc::ServerContext* context, const User* request, User* response) {
    User newUser;
    newUser.set_name(request->name());

    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::tm tm_struct = *std::localtime(&now);
    std::stringstream bufferstr;
    bufferstr << std::put_time(&tm_struct, "%Y-%m-%d %H:%M:%S");
    std::string buffer = bufferstr.str();
    newUser.set_created_at(buffer);
    newUser.set_updated_at(buffer);

    pqxx::work txn(database->get_connection());

    txn.exec("INSERT INTO users (name, created_at, updated_at) VALUES ( '" + newUser.name() +
             "', NOW(), NOW())");

    User getUser;

    pqxx::result res = txn.exec("SELECT * FROM users ORDER BY id DESC LIMIT 1");

    txn.commit();

    for (auto row : res) {
        nlohmann::json json_data = {{"method", "POST"},
                                    {"data",
                                     {
                                         {"id", row["id"].as<int64_t>()},
                                         {"name", row["name"].c_str()},
                                         {"created_at", row["created_at"].c_str()},
                                         {"updated_at", row["updated_at"].c_str()},
                                     }}};

        getUser.set_id(row["id"].as<int64_t>());
        getUser.set_name(row["name"].c_str());
        getUser.set_created_at(row["created_at"].c_str());
        getUser.set_updated_at(row["updated_at"].c_str());

        std::string json_response = json_data.dump();

        publisher->publish_message(json_response);
    }

    *response = getUser;

    return Status::OK;
};

grpc::Status UserServiceImpl::ReadUser(grpc::ServerContext* context, const Int64Value* request, User* response) {
    int64_t userId = request->value();

    pqxx::work txn(database->get_connection());

    pqxx::result res = txn.exec("SELECT * FROM users WHERE id = " + std::to_string(userId));

    txn.commit();

    if (res.empty()) {
        return Status(grpc::StatusCode::NOT_FOUND, "User not found");
    }

    User user;

    for (auto row : res) {
        nlohmann::json json_data = {{"id", row["id"].as<int64_t>()},
                                    {"name", row["name"].c_str()},
                                    {"created_at", row["created_at"].c_str()},
                                    {"updated_at", row["updated_at"].c_str()}};

        user.set_id(row["id"].as<int64_t>());
        user.set_name(row["name"].c_str());
        user.set_created_at(row["created_at"].c_str());
        user.set_updated_at(row["updated_at"].c_str());
    }

    *response = user;

    return Status::OK;
};

grpc::Status UserServiceImpl::UpdateUser(grpc::ServerContext* context, const User* request, User* response) {
    int64_t userId = request->id();

    pqxx::work txn(database->get_connection());

    pqxx::result result = txn.exec("SELECT * FROM users WHERE id = " + std::to_string(userId));

    if (result.empty()) {
        return Status(grpc::StatusCode::NOT_FOUND, "User not found");
    }

    txn.exec("UPDATE users SET name='" + request->name() +
             "', updated_at= NOW() WHERE id = " + std::to_string(userId));

    User getUser;

    pqxx::result res = txn.exec("SELECT * FROM users ORDER BY updated_at DESC LIMIT 1");

    txn.commit();

    for (auto row : res) {
        nlohmann::json json_data = {{"method", "PUT"},
                                    {"data",
                                     {
                                         {"id", row["id"].as<int64_t>()},
                                         {"name", row["name"].c_str()},
                                         {"created_at", row["created_at"].c_str()},
                                         {"updated_at", row["updated_at"].c_str()},
                                     }}};

        getUser.set_id(row["id"].as<int64_t>());
        getUser.set_name(row["name"].c_str());
        getUser.set_created_at(row["created_at"].c_str());
        getUser.set_updated_at(row["updated_at"].c_str());

        std::string json_response = json_data.dump();

        publisher->publish_message(json_response);
    }

    *response = getUser;

    return Status::OK;
};

grpc::Status UserServiceImpl::DeleteUser(grpc::ServerContext* context, const Int64Value* request, User* response) {
    int64_t userId = request->value();

    pqxx::work txn(database->get_connection());

    pqxx::result res = txn.exec("SELECT * FROM users WHERE id = " + std::to_string(userId));

    if (res.empty()) {
        return Status(grpc::StatusCode::NOT_FOUND, "User not found");
    }

    User user;

    for (auto row : res) {
        nlohmann::json json_data = {{"method", "DELETE"},
                                    {"data",
                                     {
                                         {"id", row["id"].as<int64_t>()},
                                         {"name", row["name"].c_str()},
                                         {"created_at", row["created_at"].c_str()},
                                         {"updated_at", row["updated_at"].c_str()},
                                     }}};

        user.set_id(row["id"].as<int64_t>());
        user.set_name(row["name"].c_str());
        user.set_created_at(row["created_at"].c_str());
        user.set_updated_at(row["updated_at"].c_str());

        std::string json_response = json_data.dump();

        publisher->publish_message(json_response);
    }

    txn.exec("DELETE FROM users WHERE id = " + std::to_string(userId));

    txn.commit();

    *response = user;

    return Status::OK;
};

grpc::Status UserServiceImpl::ListUsers(grpc::ServerContext* context, const Empty* request, grpc::ServerWriter<User>* writer) {
    pqxx::work txn(database->get_connection());

    pqxx::result res = txn.exec("SELECT * FROM users");

    txn.commit();

    if (res.empty()) {
        return Status(grpc::StatusCode::NOT_FOUND, "User not found");
    }

    for (auto row : res) {
        User user;
        
        user.set_id(row["id"].as<int64_t>());
        user.set_name(row["name"].c_str());
        user.set_created_at(row["created_at"].c_str());
        user.set_updated_at(row["updated_at"].c_str());

        writer->Write(user);
    }

    return Status::OK;
};