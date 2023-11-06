#pragma once

class ThreadsContainer {
  public:
    static void publisher_thread();
    static void subscriber_thread();
    static void sse_server_thread();
};