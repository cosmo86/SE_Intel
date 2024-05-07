#ifndef SERVICE_HPP
#define SERVICE_HPP
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <string>
#include <vector>
#include "TORATstpLev2ApiStruct.h"
#include "TORATstpLev2ApiDataType.h"
#include "TORATstpLev2MdApi.h"
#include <nlohmann/json.hpp>

// for convenience
using json = nlohmann::json;
using namespace TORALEV2API;
using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
typedef websocketpp::server<websocketpp::config::asio> server;
class service
{
private:
    server echo_server;
    std::vector<websocketpp::connection_hdl> socked_V;
    static std::string timestampToString(std::time_t timestamp);

public:
    service() : echo_server(), socked_V() {}
    ~service() {}

public:
    void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg);
    void on_open(websocketpp::connection_hdl hdl);
    void on_close(websocketpp::connection_hdl hdl);
    void sendTransaction(CTORATstpLev2TransactionField *pTransaction);
    void sendOrderDetail(CTORATstpLev2OrderDetailField *pOrderDetail);
    void sendNGTSTick(CTORATstpLev2NGTSTickField *pTick);
    void sendMarketData(CTORATstpLev2MarketDataField *pMarketData);

    void service_init();
};
#endif