#include <iostream>
#include <thread>
#include "./globals/globals.h"
#include "./grpc/user_grpc_handler.h"
#include "./threads/threads_container.h"

using namespace std;

/* Global variables and objects definition */
Database *database;
Publisher *publisher;
Subscriber *subscriber;
SSEServer *sse_server;
std::vector<std::string> message_queue;
/* End of definition */

void RunApp(){
    try {
        cout << "============ DB Conn & Create Table ===========" << endl;
        database = new Database();
        database->create_table();
        pqxx::connection &db_connection = database->get_connection();
        cout << db_connection.connection_string() << endl;
        cout << "=================================================" << endl;

        cout << "=================== APP GRPC Server =================" << endl;
        string server_address("localhost:5000");
        UserServiceImpl service;

        ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);

        unique_ptr<Server> server(builder.BuildAndStart());
        cout << "Server listening on http://" << server_address << endl;
        cout << "==================================================" << endl;
        
        publisher = new Publisher();
        subscriber = new Subscriber();
        sse_server = new SSEServer();

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
