#pragma once

#include <chrono>
#include <cstdlib>
#include <map>
#include <memory>
#include <restbed>
#include <string>
#include "iostream"

using namespace std;
using namespace restbed;
using namespace std::chrono;

class SSEServer {
  public:
    void start_sse();
    void event_stream_handler(string message);

  private:
    void register_event_source_handler(const std::shared_ptr<Session> session);

    std::vector<std::shared_ptr<Session>> sessions;
};
