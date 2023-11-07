#include "sse_handler.h"
#include "../globals/globals.h"

using namespace std;
using namespace restbed;
using namespace std::chrono;

void SSEServer::register_event_source_handler(const std::shared_ptr<Session> session) {
    const auto headers =
        std::multimap<std::string, std::string>{{"Connection", "keep-alive"},
                                                {"Cache-Control", "no-cache"},
                                                {"Content-Type", "text/event-stream"},
                                                {"Access-Control-Allow-Origin", "*"}};

    session->yield(OK, headers, [this](const std::shared_ptr<Session> session) {
        this->sessions.push_back(session);
    });
}

void SSEServer::event_stream_handler(string message) {
    try {
        sessions.erase(
            std::remove_if(sessions.begin(), sessions.end(),
                           [](const std::shared_ptr<Session>& a) { return a->is_closed(); }),
            sessions.end());

        for (const auto& session : sessions) {
            const auto message_to_broadcast = "data: " + message + "\n\n";
            session->yield(message_to_broadcast);
            std::cout << "Broadcasted Message in SSE: " << message_to_broadcast << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void SSEServer::start_sse() {
    auto resource = std::make_shared<Resource>();
    resource->set_path("/stream");
    resource->set_method_handler(
        "GET", std::bind(&SSEServer::register_event_source_handler, this, std::placeholders::_1));

    auto settings = std::make_shared<Settings>();
    auto sse_port = stoi(env_reader->get("SSE_SERVER_PORT"));
    settings->set_port(sse_port);

    auto service = std::make_shared<Service>();
    service->publish(resource);
    service->start(settings);
}
