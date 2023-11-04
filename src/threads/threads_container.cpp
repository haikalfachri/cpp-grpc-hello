#include "threads_container.h"

#include "../globals/globals.h"

void ThreadsContainer::publisher_thread() {
    publisher = new Publisher();
}

void ThreadsContainer::subscriber_thread() {
    subscriber = new Subscriber();
    subscriber->subscriber_listening();
}

void ThreadsContainer::sse_server_thread() {
    sse_server = new SSEServer();
    sse_server->start_sse();
}
