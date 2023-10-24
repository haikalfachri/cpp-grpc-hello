#include <iostream>
#include <string>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;

int main()
{
    io_service io;
    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 8080));
    std::cout << "Server is running on http://localhost:8080" << std::endl;
    while (true)
    {
        tcp::socket socket(io);
        acceptor.accept(socket);

        std::string response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: text/event-stream\r\n";
        response += "Cache-Control: no-cache\r\n";
        response += "Connection: keep-alive\r\n";
        response += "\r\n";

        socket.write_some(buffer(response));

        while (true)
        {
            // Kirim peristiwa SSE ke klien
            std::string event = "data: Pesan SSE\r\n\r\n";
            std::cout << event << std::endl;
            socket.write_some(buffer(event));
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    }

    return 0;
}
