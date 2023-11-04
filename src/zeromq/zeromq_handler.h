#pragma once

#include <vector>
#include <zmq.hpp>
#include <zmq_addon.hpp>
#include "iostream"

using namespace std;

class Publisher {
  private:
    zmq::context_t context;
    zmq::socket_t publisher;

  public:
    Publisher();
    void publish_message(const std::string &message);
    void publisher_pubslishing();
};

class Subscriber {
  private:
    zmq::context_t context;
    zmq::socket_t subscriber;

  public:
    Subscriber();
    std::string receive_message();
    bool is_connected();
    void subscriber_listening();
};
