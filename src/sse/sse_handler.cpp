#include "sse_handler.h"
#include "../globals/globals.h"

// vector<shared_ptr<Session>> sessions;

// void register_event_source_handler(const shared_ptr<Session> session) {
//     const auto headers = multimap<string, string>{
//         {"Connection", "keep-alive"},
//         {"Cache-Control", "no-cache"},
//         {"Content-Type", "text/event-stream"},
//         {"Access-Control-Allow-Origin", "*"}
//     };

//     session->yield(OK, headers,
//                     [](const shared_ptr<Session> session) { sessions.push_back(session); });
// }

// void event_stream_handler(void) {
//     try {
//         sessions.erase(std::remove_if(sessions.begin(), sessions.end(),
//                                     [](const shared_ptr<Session>& a) { return a->is_closed();
//                                     }),
//                         sessions.end());

//         for (const auto& session : sessions) {
//             if (!messageQueue.empty()) {
//                 const auto message = "data: " + messageQueue.front() + "\n\n";
//                 session->yield(message);
//                 cout << "Broadcasted Message in SSE: " << message << endl;
//             }
//         }

//         if (!messageQueue.empty()) {
//             messageQueue.pop();
//         }
//     } catch (const exception& e) {
//         cerr << "Exception: " << e.what() << endl;
//     }
// }

// void run_sse() {
//     auto resource = make_shared<Resource>();
//     resource->set_path("/stream");
//     resource->set_method_handler("GET", register_event_source_handler);

//     auto settings = make_shared<Settings>();
//     settings->set_port(5003);

//     auto service = make_shared<Service>();
//     service->publish(resource);
//     service->schedule(event_stream_handler, milliseconds(1000));
//     service->start(settings);
// }

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

void SSEServer::event_stream_handler() {
    try {
        sessions.erase(
            std::remove_if(sessions.begin(), sessions.end(),
                           [](const std::shared_ptr<Session>& a) { return a->is_closed(); }),
            sessions.end());

        for (const auto& session : sessions) {
            if (!message_queue.empty()) {
                const auto message = "data: " + message_queue.front() + "\n\n";
                session->yield(message);
                std::cout << "Broadcasted Message in SSE: " << message << std::endl;
            }
        }

        if (!message_queue.empty()) {
            message_queue.pop_back();
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void SSEServer::start_sse() {
    auto resource = std::make_shared<Resource>();
    resource->set_path("/stream");
    resource->set_method_handler("GET", std::bind(&SSEServer::register_event_source_handler, this, std::placeholders::_1));

    auto settings = std::make_shared<Settings>();
    settings->set_port(5003);

    auto service = std::make_shared<Service>();
    service->publish(resource);
    service->schedule(std::bind(&SSEServer::event_stream_handler, this),
                      std::chrono::milliseconds(1000));
    service->start(settings);
}
