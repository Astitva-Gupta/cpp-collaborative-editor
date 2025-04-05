#include "editor_server.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <set>
#include <map>
#include <mutex>
#include <memory>
#include <thread>

using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;
using ws_stream = websocket::stream<tcp::socket>;

boost::asio::io_context ioc;
std::set<ws_stream*> clients;
std::map<ws_stream*, std::string> usernames;
std::mutex editing_mutex;
bool is_being_edited = false;
ws_stream* current_editor = nullptr;

void broadcast(const std::string& message, ws_stream* sender = nullptr) {
    for (auto client : clients) {
        if (client != sender) {
            boost::beast::error_code ec;
            client->write(boost::asio::buffer(message), ec);
        }
    }
}

void handle_session(std::shared_ptr<ws_stream> ws) {
    try {
        ws->accept();
        {
            std::lock_guard<std::mutex> lock(editing_mutex);
            clients.insert(ws.get());
        }

        boost::beast::flat_buffer buffer;
        
        // First message is the username
        ws->read(buffer);
        std::string username = boost::beast::buffers_to_string(buffer.data());
        buffer.consume(buffer.size());

        {
            std::lock_guard<std::mutex> lock(editing_mutex);
            usernames[ws.get()] = username;
        }

        std::cout << username << " connected.\n";

        while (true) {
            buffer.consume(buffer.size());
            ws->read(buffer);
            std::string msg = boost::beast::buffers_to_string(buffer.data());

            std::lock_guard<std::mutex> lock(editing_mutex);
            if (msg == "start") {
                if (!is_being_edited) {
                    is_being_edited = true;
                    current_editor = ws.get();
                    ws->write(boost::asio::buffer("LOCK_GRANTED"));
                    broadcast(username + " is editing", ws.get());
                } else {
                    ws->write(boost::asio::buffer("LOCK_DENIED"));
                }
            } else if (msg == "stop") {
                if (ws.get() == current_editor) {
                    is_being_edited = false;
                    current_editor = nullptr;
                    broadcast(username + " stopped editing", ws.get());
                }
            } else {
                broadcast(username + ": " + msg, ws.get()); // message content
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Session ended: " << e.what() << std::endl;
    }

    {
        std::lock_guard<std::mutex> lock(editing_mutex);
        if (ws.get() == current_editor) {
            is_being_edited = false;
            broadcast(usernames[ws.get()] + " disconnected. Lock released.");
        }
        clients.erase(ws.get());
        usernames.erase(ws.get());
    }
}

void run_server(const std::string& address, unsigned short port) {
    try {
        tcp::acceptor acceptor(ioc, tcp::endpoint(boost::asio::ip::make_address(address), port));
        std::cout << "Server started on " << address << ":" << port << "...\n";

        while (true) {
            tcp::socket socket(ioc);
            acceptor.accept(socket);

            auto ws = std::make_shared<ws_stream>(std::move(socket));
            std::thread{[ws]() { handle_session(ws); }}.detach();
        }

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
    }
}
