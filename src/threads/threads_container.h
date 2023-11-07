#pragma once

#include <string>
#include <iostream>

class ThreadsContainer {
  public:
    static void publisher_thread();
    static void subscriber_thread();
    static void sse_server_thread();
    static void send_to_sse_thread(std::string message);
};