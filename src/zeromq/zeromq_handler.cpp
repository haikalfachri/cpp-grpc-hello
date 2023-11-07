#include "zeromq_handler.h"
#include "../globals/globals.h"
#include "../grpc/user_grpc_handler.h"

using user::User;
using namespace std;

Publisher::Publisher() : context(1), publisher(context, ZMQ_PUB) {
    string pub_url = env_reader->get("ZMQ_PUB_URL");
    string pub_port = env_reader->get("ZMQ_PUB_PORT");
    string pub_address = pub_url + ":" + pub_port;
    publisher.bind("tcp://" + pub_address);
}

void Publisher::publish_message(const std::string &message) {
    try {
        publisher.send(zmq::str_buffer("user-topic"), zmq::send_flags::sndmore);
        publisher.send(zmq::buffer(message));
    } catch (const exception &e) {
        cerr << "Exception caught: " << e.what() << endl;
    }
}

Subscriber::Subscriber() : context(1), subscriber(context, ZMQ_SUB) {
    string sub_url = env_reader->get("ZMQ_SUB_URL");
    string sub_port = env_reader->get("ZMQ_SUB_PORT");
    string sub_address = sub_url + ":" + sub_port;
    subscriber.connect("tcp://" + sub_address);
    subscriber.set(zmq::sockopt::subscribe, "user-topic");
}

std::vector<zmq::message_t> Subscriber::receive_message() {
    std::vector<zmq::message_t> recv_msgs;
    zmq::recv_result_t result = zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
    return recv_msgs;
}

bool Subscriber::is_connected() {
    return subscriber.connected();
}

void Subscriber::subscriber_listening() {
    try {
        if (this->is_connected()) {
            cout << "Subscriber listen!" << endl;
        }

        while (true) {
            std::vector<zmq::message_t> recv_msgs = this->receive_message();
            if (recv_msgs.size() > 0) {
                string message = recv_msgs[1].to_string();
                thread send_to_sse_thread(ThreadsContainer::send_to_sse_thread, message);
                send_to_sse_thread.join();
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

            string sub_url = env_reader->get("ZMQ_SUB_URL");
            string sub_port = env_reader->get("ZMQ_SUB_PORT");
            string sub_address = sub_url + ":" + sub_port;

            string pub_url = env_reader->get("ZMQ_PUB_URL");
            string pub_port = env_reader->get("ZMQ_PUB_PORT");
            string pub_address = pub_url + ":" + pub_port;

            if (pub_port != sub_port) {
                pqxx::work txn(database->get_connection());

                std::string method = data["method"].GetString();
                std::string postMethod = "POST";
                std::string putMethod = "PUT";
                std::string deleteMethod = "DELETE";

                if (method == postMethod) {
                    txn.exec("INSERT INTO users (name, created_at, updated_at) VALUES ('" +
                             user.name() + "', '" + user.created_at() + "', '" +
                             user.updated_at() + "')");
                } else if (method == putMethod) {
                    txn.exec("UPDATE users SET name='" + user.name() +
                             "', updated_at= NOW() WHERE id = " + to_string(user.id()));
                } else if (method == deleteMethod) {
                    txn.exec("DELETE FROM users WHERE id = " + to_string(user.id()));
                }

                txn.commit();
            }
        }
    } catch (const exception &e) {
        cerr << "Exception caught: " << e.what() << endl;
    }
}
