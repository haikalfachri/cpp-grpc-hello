#pragma once

#include <vector>
#include <zmq.hpp>
#include <zmq_addon.hpp>
#include "../threads/threads_container.h"
#include "iostream"

class Publisher {
  private:
    zmq::context_t context;
    zmq::socket_t publisher;

  public:
    Publisher();
    void publish_message(const std::string &message);
};

class Subscriber {
  private:
    zmq::context_t context;
    zmq::socket_t subscriber;

  public:
    Subscriber();
    std::vector<zmq::message_t> receive_message();
    bool is_connected();
    void subscriber_listening();
};
