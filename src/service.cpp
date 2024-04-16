#include <iostream>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

// Define a server type
typedef websocketpp::server<websocketpp::config::asio> server;

int main() {
    // Create a server instance
    server srv;

    // Set logging level
    srv.set_access_channels(websocketpp::log::alevel::all);
    srv.clear_access_channels(websocketpp::log::alevel::frame_payload);

    // Define a callback to handle connections
    srv.set_open_handler([&srv](websocketpp::connection_hdl hdl) {
        std::cout << "Client connected" << std::endl;

        try {
            srv.send(hdl, "Welcome to the WebSocket server", websocketpp::frame::opcode::text);
        } catch (const std::exception& e) {
            std::cerr << "Exception while sending welcome message: " << e.what() << std::endl;
        }
    });

    // Initialize Asio
    srv.init_asio();

    // Listen on port 9002
    srv.listen(9002);

    // Start accepting connections
    srv.start_accept();

    // Start the Asio event loop
    srv.run();

    return 0;
}
