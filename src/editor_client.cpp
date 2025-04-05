#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <atomic>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;

std::atomic<bool> running{true};

void read_loop(websocket::stream<tcp::socket>& ws) {
    try {
        beast::flat_buffer buffer;
        while (running) {
            buffer.consume(buffer.size());
            ws.read(buffer);
            std::string msg{static_cast<const char*>(buffer.data().data()), buffer.size()};
            std::cout << "\n[Server] " << msg << "\n> ";
        }
    } catch (...) {
        std::cout << "\n[Info] Disconnected from server.\n";
        running = false;
    }
}

int main() {
    try {
        net::io_context ioc;
        tcp::resolver resolver{ioc};
        auto const results = resolver.resolve("127.0.0.1", "9002");

        websocket::stream<tcp::socket> ws{ioc};
        net::connect(ws.next_layer(), results.begin(), results.end());
        ws.handshake("127.0.0.1", "/");

        std::string username;
        std::cout << "Enter your username: ";
        std::getline(std::cin, username);
        ws.write(net::buffer(username)); // optional, just sent as-is

        std::thread reader(read_loop, std::ref(ws));

        std::string input;
        std::cout << "Commands: 'start' (lock), 'stop' (unlock), or type your message:\n> ";

        while (running && std::getline(std::cin, input)) {
            if (input == "start") {
                ws.write(net::buffer("Started Editing"));
            } else if (input == "stop") {
                ws.write(net::buffer("Stopped Editing"));
            } else {
                ws.write(net::buffer(input));
            }
            std::cout << "> ";
        }

        running = false;
        if (reader.joinable()) reader.join();

    } catch (const std::exception& e) {
        std::cerr << "[Error] Client exception: " << e.what() << "\n";
    }

    return 0;
}
