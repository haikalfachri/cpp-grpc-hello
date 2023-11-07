#pragma once

#include <chrono>
#include <cstdlib>
#include <map>
#include <memory>
#include <restbed>
#include <string>
#include "iostream"

class SSEServer {
  public:
    void start_sse();
    void event_stream_handler(std::string message);

  private:
    void register_event_source_handler(const std::shared_ptr<restbed::Session> session);

    std::vector<std::shared_ptr<restbed::Session>> sessions;
};
