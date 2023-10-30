#include <grpcpp/grpcpp.h>

#include <boost/asio.hpp>
#include <chrono>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <pqxx/pqxx>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "rapidjson/document.h"
#include "user.grpc.pb.h"
#include "user.pb.h"

using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

using google::protobuf::Empty;
using google::protobuf::Int64Value;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using ip::tcp;
using user::User;
using user::UserService;

zmq::context_t context{1};
zmq::socket_t publisher{context, zmq::socket_type::pub};
zmq::socket_t subscriber{context, zmq::socket_type::sub};
std::string message;
std::queue<std::string> messageQueue;

string appURL = "localhost";
string pubsURL = "*";
string subsURL = "localhost";
string pubsPort = "5002";
string subsPort = "5052";
string ssePort = "5001";
string appPort = "5000";
pqxx::connection c("postgresql://postgres:postgres@localhost:5433/cpp-grpc-crud");

class UserServiceImpl final : public UserService::Service {
  public:
    Status CreateUser(ServerContext *context, const User *request, User *response) override {
        User newUser;
        newUser.set_name(request->name());

        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        std::tm tm_struct = *std::localtime(&now);
        std::stringstream bufferstr;
        bufferstr << std::put_time(&tm_struct, "%Y-%m-%d %H:%M:%S");
        std::string buffer = bufferstr.str();
        newUser.set_created_at(buffer);
        newUser.set_updated_at(buffer);

        pqxx::work txn(c);

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

            publisher.send(zmq::str_buffer("Hello"), zmq::send_flags::sndmore);
            publisher.send(zmq::buffer(json_response));
        }

        *response = getUser;

        return Status::OK;
    }

    Status ReadUser(ServerContext *context, const Int64Value *request, User *response) override {
        int64_t userId = request->value();

        pqxx::work txn(c);

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
    }

    Status UpdateUser(ServerContext *context, const User *request, User *response) override {
        int64_t userId = request->id();

        pqxx::work txn(c);

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

            publisher.send(zmq::str_buffer("Hello"), zmq::send_flags::sndmore);
            publisher.send(zmq::buffer(json_response));
        }

        *response = getUser;

        return Status::OK;
    }

    Status DeleteUser(ServerContext *context, const Int64Value *request, User *response) override {
        int64_t userId = request->value();

        pqxx::work txn(c);

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

            publisher.send(zmq::str_buffer("Hello"), zmq::send_flags::sndmore);
            publisher.send(zmq::buffer(json_response));
        }

        txn.exec("DELETE FROM users WHERE id = " + std::to_string(userId));

        txn.commit();

        *response = user;

        return Status::OK;
    }

    Status ListUsers(ServerContext *context, const Empty *request,
                     grpc::ServerWriter<User> *writer) override {
        pqxx::work txn(c);

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
    }
};

void StartDB() {
    pqxx::work txn{c};

    txn.exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            id SERIAL PRIMARY KEY,
            name VARCHAR(50) NOT NULL,
            created_at TIMESTAMP NOT NULL DEFAULT NOW(),
            updated_at TIMESTAMP NOT NULL DEFAULT NOW()
        )
    )");

    cout << "Table created successfully" << endl;

    txn.commit();
}

void RunPublisher() {
    publisher.bind("tcp://" + pubsURL + ":" + pubsPort);
}

void RunSubscriber() {
    subscriber.connect("tcp://" + subsURL + ":" + subsPort);
    subscriber.set(zmq::sockopt::subscribe, "Hello");

    if (subscriber.connected()) {
        std::cout << "Subscriber listen!" << std::endl;
    }

    while (true) {
        std::vector<zmq::message_t> recv_msgs;
        zmq::recv_result_t result = zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
        if (!(result && "recv failed") || !(*result == 2)) {
            continue;
        }

        if (recv_msgs.size() > 0) {
            std::string message = recv_msgs[1].to_string();
            messageQueue.push(message);
            std::cout << "MessageQueue pushed: " << message << std::endl;
            std::cout << "Subscriber received: " << recv_msgs[1].to_string() << std::endl;
        }

        rapidjson::Document data;
        std::string recvMessage = recv_msgs[1].to_string();
        data.Parse(recvMessage.c_str());

        User user;
        if (data.HasMember("data")) {
            const auto &jsonData = data["data"];
            if (jsonData.HasMember("id") && jsonData["id"].IsInt64()) {
                user.set_id(jsonData["id"].GetInt64());
            }

            if (jsonData.HasMember("name") && jsonData["name"].IsString()) {
                user.set_name(jsonData["name"].GetString());
            }

            if (jsonData.HasMember("created_at") && jsonData["created_at"].IsString()) {
                user.set_created_at(jsonData["created_at"].GetString());
            }

            if (jsonData.HasMember("updated_at") && jsonData["updated_at"].IsString()) {
                user.set_updated_at(jsonData["updated_at"].GetString());
            }
        }

        pqxx::work txn(c);

        std::string method = data["method"].GetString();
        std::string postMethod = "POST";
        std::string putMethod = "PUT";
        std::string deleteMethod = "DELETE";

        if (method == postMethod) {
            txn.exec("INSERT INTO users (id, name, created_at, updated_at) VALUES (" +
                     std::to_string(user.id()) + ", '" + user.name() + "', '" + user.created_at() +
                     "', '" + user.updated_at() + "')");
        } else if (method == putMethod) {
            txn.exec("UPDATE users SET name='" + user.name() +
                     "', updated_at= NOW() WHERE id = " + to_string(user.id()));
        } else if (method == deleteMethod) {
            txn.exec("DELETE FROM users WHERE id = " + to_string(user.id()));
        }

        txn.commit();
    }
}

void RunSSE() {
    io_service io;
    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), stoi(ssePort)));
    std::cout << "SSE server is running on " << appURL << ":" << ssePort << std::endl;

    tcp::socket socket(io);
    acceptor.accept(socket);

    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/event-stream\r\n";
    response += "Cache-Control: no-cache\r\n";
    response += "Connection: keep-alive\r\n";
    response += "\r\n";

    socket.write_some(buffer(response));

    // while (true) {
    //     std::mutex mtx;
    //     std::condition_variable cv;

    //     std::unique_lock<std::mutex> lock(mtx);
    //     cv.wait(lock, [&] { return !messageQueue.empty(); });

    //     std::string message = messageQueue.front();

    //     message = "data: " + message + "\r\n\r\n";
    //     socket.write_some(buffer(message));
    //     std::cout << "Broadcasted Message in SSE: " << message << std::endl;

    //     messageQueue.pop();
    // }
    while (true) {
        if (!messageQueue.empty()) {
            std::string message = messageQueue.front();
            messageQueue.pop();
            std::string messageToBroadcast = "data: " + message + "\r\n\r\n";
            socket.write_some(buffer(messageToBroadcast));
            std::cout << "Broadcasted Message in SSE: " << message << std::endl;
        }
    }
}

void RunServer() {
    try {
        std::string server_address(appURL + ":" + appPort);
        UserServiceImpl service;

        ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);

        std::unique_ptr<Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << server_address << std::endl;

        StartDB();
        std::thread publisherThread(RunPublisher);
        std::thread subscriberThread(RunSubscriber);
        std::thread sseServerThread(RunSSE);
        server->Wait();

        publisherThread.join();
        subscriberThread.join();
        sseServerThread.join();
    } catch (const std::exception &e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}

int main(int argc, char **argv) {
    RunServer();
    return 0;
}
