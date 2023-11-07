#include <iostream>
#include <thread>
#include "./globals/globals.h"
#include "./grpc/user_grpc_handler.h"
#include "./threads/threads_container.h"

using user::UserService;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using namespace std;

/* Global variables and objects definition */
Database *database;
Publisher *publisher;
Subscriber *subscriber;
SSEServer *sse_server;
EnvReader *env_reader;
/* End of definition */

void RunApp() {
    try {
        env_reader = new EnvReader("../../.env");

        database = new Database();
        pqxx::connection &db_connection = database->get_connection();
        cout << "Data Source Name (DSN): " << db_connection.connection_string() << endl;
        database->create_table();

        string app_url = env_reader->get("APP_URL");
        string app_port = env_reader->get("APP_PORT");

        string server_address = app_url + ":" + app_port;
        UserServiceImpl service;

        ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);

        unique_ptr<Server> server(builder.BuildAndStart());
        cout << "Server listening on http://" << server_address << endl;
        cout << "SSE Server listening on http://" + env_reader->get("APP_URL") + ":" +
                    env_reader->get("SSE_SERVER_PORT") + "/stream"
             << endl;

        thread publisher_thread(ThreadsContainer::publisher_thread);
        thread subscriber_thread(ThreadsContainer::subscriber_thread);
        thread sse_server_thread(ThreadsContainer::sse_server_thread);
        server->Wait();

        publisher_thread.join();
        subscriber_thread.join();
        sse_server_thread.join();

    } catch (const exception &e) {
        cerr << "Exception caught: " << e.what() << endl;
    }
}

int main(int argc, char **argv) {
    RunApp();
    return 0;
}
