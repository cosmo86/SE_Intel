#include <iostream>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <string>
#include <vector>
#include "/root/vcpkg/packages/jsoncpp_x64-linux/include/json/json.h"
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
typedef websocketpp::server<websocketpp::config::asio> server;
class service{
    private:
        server echo_server;
        std::vector<websocketpp::connection_hdl> socked_V;
        static std::string timestampToString(std::time_t timestamp);
    public:
        service():echo_server(),socked_V(){}
        ~service(){}
    public:
    void on_message( websocketpp::connection_hdl hdl, server::message_ptr msg);
    // Callback function when a new connection is established
    void on_open( websocketpp::connection_hdl hdl);
    void on_close( websocketpp::connection_hdl hdl);
    void service_init();
};
            