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

  private:
    void register_event_source_handler(const std::shared_ptr<Session> session);
    void event_stream_handler();

    std::vector<std::shared_ptr<Session>> sessions;
};
