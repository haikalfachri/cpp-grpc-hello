#include "zeromq_handler.h"

Publisher::Publisher() : context(1), publisher(context, ZMQ_PUB) {
    publisher.bind("tcp://*:5002");
}

void Publisher::publish_message(const std::string &message) {
    publisher.send(zmq::str_buffer("user-topic"), zmq::send_flags::sndmore);
    publisher.send(zmq::buffer(message));
}

void Publisher::publisher_pubslishing() {
    publisher.bind("tcp://*:5002");
}

// void Publisher::publish_message(const std::string &message) {
//     zmq::message_t zmqMessage(message.size());
//     memcpy(zmqMessage.data(), message.data(), message.size());
//     publisher.send(zmqMessage, zmq::send_flags::dontwait);
// }

Subscriber::Subscriber() : context(1), subscriber(context, ZMQ_SUB) {
    subscriber.connect("tcp://localhost:5002");
    subscriber.set(zmq::sockopt::subscribe, "user-topic");
}

std::string Subscriber::receive_message() {
    zmq::message_t zmqMessage;
    subscriber.recv(zmqMessage);
    return std::string(static_cast<char *>(zmqMessage.data()), zmqMessage.size());
}

bool Subscriber::is_connected() {
    return subscriber.connected();
}

void Subscriber::subscriber_listening() {
    if (this->is_connected()) {
        cout << "Subscriber listen!" << endl;
    }

    while (true) {
        cout << "Subscriber received: "<< this->receive_message() << endl;
    }
}

// void RunPublisher() {
//     // publisher.bind("tcp://" + pubsURL + ":" + pubsPort);
//     publisher.bind("tcp://*:5002");
// }

// void RunSubscriber() {
//     subscriber.connect("tcp://localhost:5002");
//     subscriber.set(zmq::sockopt::subscribe, "Hello");

//     if (subscriber.connected()) {
//         std::cout << "Subscriber listen!" << std::endl;
//     }

//     while (true) {
//         std::vector<zmq::message_t> recv_msgs;
//         zmq::recv_result_t result = zmq::recv_multipart(subscriber,
//         std::back_inserter(recv_msgs)); if (!(result && "recv failed") || !(*result == 2)) {
//             continue;
//         }

//         if (recv_msgs.size() > 0) {
//             message = recv_msgs[1].to_string();
//             messageQueue.push(message);
//             std::cout << "MessageQueue pushed: " << message << std::endl;
//             std::cout << "Subscriber received: " << recv_msgs[1].to_string() << std::endl;
//         }

//         rapidjson::Document data;
//         std::string recvMessage = recv_msgs[1].to_string();
//         data.Parse(recvMessage.c_str());

//         User user;
//         if (data.HasMember("data")) {
//             const auto &jsonData = data["data"];
//             if (jsonData.HasMember("id") && jsonData["id"].IsInt64()) {
//                 user.set_id(jsonData["id"].GetInt64());
//             }

//             if (jsonData.HasMember("name") && jsonData["name"].IsString()) {
//                 user.set_name(jsonData["name"].GetString());
//             }

//             if (jsonData.HasMember("created_at") && jsonData["created_at"].IsString()) {
//                 user.set_created_at(jsonData["created_at"].GetString());
//             }

//             if (jsonData.HasMember("updated_at") && jsonData["updated_at"].IsString()) {
//                 user.set_updated_at(jsonData["updated_at"].GetString());
//             }
//         }

//         pqxx::work txn(c);

//         std::string method = data["method"].GetString();
//         std::string postMethod = "POST";
//         std::string putMethod = "PUT";
//         std::string deleteMethod = "DELETE";

//         if (method == postMethod) {
//             txn.exec("INSERT INTO users (id, name, created_at, updated_at) VALUES (" +
//                      std::to_string(user.id()) + ", '" + user.name() + "', '" +
//                      user.created_at() +
//                      "', '" + user.updated_at() + "')");
//         } else if (method == putMethod) {
//             txn.exec("UPDATE users SET name='" + user.name() +
//                      "', updated_at= NOW() WHERE id = " + to_string(user.id()));
//         } else if (method == deleteMethod) {
//             txn.exec("DELETE FROM users WHERE id = " + to_string(user.id()));
//         }

//         txn.commit();
//     }
// }
