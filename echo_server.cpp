#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <iostream>
#include <thread>
#include <sstream>

typedef websocketpp::server<websocketpp::config::asio> server;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

// Define a callback to handle incoming messages
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;

    // check for a special command to instruct the server to stop listening so
    // it can be cleanly exited.
    if (msg->get_payload() == "stop-listening") {
        s->stop_listening();
        return;
    }

    try {
        s->send(hdl, msg->get_payload(), msg->get_opcode());
    } catch (websocketpp::exception const & e) {
        std::cout << "Echo failed because: "
                  << "(" << e.what() << ")" << std::endl;
    }
}

int main() {
    // Create a server endpoint
    server echo_server;

    try {
        // Set logging settings
        echo_server.set_access_channels(websocketpp::log::alevel::all);
        echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize Asio
        echo_server.init_asio();

        // Register our message handler
        // echo_server.set_message_handler(bind(&on_message,&echo_server,::_1,::_2));
        echo_server.set_message_handler([&](auto hdl, auto msg)
        {
            on_message(&echo_server, hdl, msg);
        });

        std::thread commThread;

        echo_server.set_open_handler([&](auto hdl)
        {
            if (!commThread.joinable())
            {
                commThread = std::thread([&, hdl]{
                    int i = 0;
                    for (;;)
                    {
                        sleep(5);
                        std::stringstream message;
                        message << "Hallo, Welt! " << i++;
                        echo_server.send(hdl, message.str(), websocketpp::frame::opcode::text);
                    }
                });
            }
        });

        echo_server.set_close_handler([&](auto hdl)
        {
            echo_server.stop_listening();
        });

        // Listen on port 9002
        echo_server.listen(9002);

        // Start the server accept loop
        echo_server.start_accept();

        // Start the ASIO io_service run loop
        echo_server.run();

        commThread.join();

    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}
