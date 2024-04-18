#include <iostream>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

using namespace std;

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

int main() {
    // 创建客户端实例
    client c;

    // 设置日志级别
    c.set_access_channels(websocketpp::log::alevel::all);
    c.clear_access_channels(websocketpp::log::alevel::frame_payload);

    // 初始化 Asio
    c.init_asio();

    // 设置 SSL 上下文，如果你的服务器使用了 SSL
    context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv1);
    c.set_tls_init_handler([ctx](websocketpp::connection_hdl) {
        return ctx;
    });

    // 指定连接地址
    string uri = "ws://91.208.73.166:9002";

    // 设置回调函数，处理连接成功事件
    c.set_open_handler([&c](websocketpp::connection_hdl hdl) {
        cout << "Connected to the server" << endl;
    });

    // 设置回调函数，处理收到消息事件
    c.set_message_handler([&c](websocketpp::connection_hdl, client::message_ptr msg) {
        cout << "Received message: " << msg->get_payload() << endl;
    });

    // 尝试连接到服务器
    websocketpp::lib::error_code ec;
    client::connection_ptr con = c.get_connection(uri, ec);
    if (ec) {
        cout << "Unable to connect to the server: " << ec.message() << endl;
        return 1;
    }

    // 启动客户端
    c.connect(con);

    // 进入事件循环
    c.run();

    return 0;
}