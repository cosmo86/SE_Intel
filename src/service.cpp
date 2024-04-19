#include "../include/service.hpp"
    std::string timestampToString(std::time_t timestamp) {
            std::time_t currentTime = std::time(nullptr);
            std::tm* tm_info = std::localtime(&currentTime);
            std::ostringstream oss;
            oss << std::put_time(tm_info, "%Y-%m-%dT%H:%M:%S");
            return oss.str();
        }
    void service::on_message( websocketpp::connection_hdl hdl, server::message_ptr msg) {
        std::cout << "on_message called with hdl: " << hdl.lock().get()
                << " and message: " << msg->get_payload()
                << std::endl;

        // Check for a special command to instruct the server to stop listening
        if (msg->get_payload() == "stop-listening") {
            echo_server.stop_listening();
            return;
        }
        std::string response = "Service response: " + msg->get_payload();
        try {
            // Send the response message back to the client
            echo_server.send(hdl, response, websocketpp::frame::opcode::text);
        } catch (const websocketpp::exception& e) {
            std::cout << "Send failed because: " << e.what() << std::endl;
        }
    }
    // Callback function when a new connection is established
    void service::on_open( websocketpp::connection_hdl hdl) {
        std::cout << "New connection established" << std::endl;
        socked_V.push_back(hdl);
        // Start a new thread to send JSON packets to the client
        //std::thread t(send_json_packet, s, hdl);
        //t.detach(); // Detach the thread to run independently
    }
    void service::on_close( websocketpp::connection_hdl hdl) {
        // Handle connection close
        // Assuming socked_V is declared outside this function and is accessible here
        for(auto it = socked_V.begin(); it != socked_V.end(); ++it) {
            if((*it).lock().get() ==hdl.lock().get()) {
                socked_V.erase(it);
                break; // Stop looping once the connection handle is erased
            }
        }
    }
    void  service::service_init(){
        try {
            // Set logging settings
            echo_server.set_access_channels(websocketpp::log::alevel::all);
            echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);
            // Initialize Asio
            echo_server.init_asio();
            echo_server.set_message_handler(bind(&service::on_message, this, _1, _2));
            echo_server.set_open_handler(bind(&service::on_open, this, _1));
            echo_server.set_close_handler(bind(&service::on_close, this, _1));
            // Listen on port 9002
            echo_server.listen(9002);
            // Start the server accept loop
            echo_server.start_accept();
            // Start the ASIO io_service run loop
            echo_server.run();
        } catch (const websocketpp::exception& e) {
            std::cout << e.what() << std::endl;
        } catch (...) {
            std::cout << "Other exception" << std::endl;
        }
    }
    // Function to send JSON packet every second
void  service::sendTransaction(CTORATstpLev2TransactionField* pTransaction){
    if(pTransaction->TradePrice==0)return;
    Json::Value val;
    val["ExchangeId"]=pTransaction->ExchangeID;
    val["ExchangeID"]=pTransaction->ExchangeID;
    val["SecurityID"]=pTransaction->SecurityID;
    val["TradeTime"]= timestampToString(pTransaction->TradeTime);
    //val["TradeTime"]=pTransaction->TradeTime;
    val["TradePrice"]=pTransaction->TradePrice;
    val["TradeVolume"]=std::to_string(pTransaction->TradeVolume);
    val["ExecType"]=pTransaction->ExecType;
    val["MainSeq"]=pTransaction->MainSeq;
    val["SubSeq"]=std::to_string(pTransaction->SubSeq);
    val["SellNo"]=std::to_string(pTransaction->SellNo);
    val["Info1"]=pTransaction->Info1;
    val["Info2"]=pTransaction->Info2;
    val["Info3"]=pTransaction->Info3;
    val["TradeBSFlag"]=pTransaction->TradeBSFlag;
    val["BizIndex"]=std::to_string(pTransaction->BizIndex);
    std::cout<<val["TradeTime"]<<std::endl;
    for(auto i:socked_V){
        try {
            // Send the JSON packet to the client
           echo_server.send(i, val.toStyledString().c_str(), websocketpp::frame::opcode::text);
        } catch (const websocketpp::exception& e) {
            std::cout << "Send failed because: " << e.what() << std::endl;
        }
    }
    //ws://91.208.73.166:9002
}
void service::sendOrderDetail(CTORATstpLev2OrderDetailField* pOrderDetail){
    Json::Value val;
    val["ExchangeID"]=pOrderDetail->ExchangeID;
    val["SecurityID"]=pOrderDetail->SecurityID;
    val["OrderTime"]=pOrderDetail->OrderTime;
    val["Price"]=pOrderDetail->Price;
    val["Volume"]=pOrderDetail->Volume;
    val["Side"]=pOrderDetail->Side;
    val["MainSeq"]=pOrderDetail->MainSeq;
    val["SubSeq"]=pOrderDetail->SubSeq;
    val["Info1"]=pOrderDetail->Info1;
    val["Info2"]=pOrderDetail->Info2;
    val["Info3"]=pOrderDetail->Info3;
    val["OrderNO"]=pOrderDetail->OrderNO;
    val["OrderStatus"]=pOrderDetail->OrderStatus;
    val["BizIndex"]=pOrderDetail->BizIndex;
    for(auto i:socked_V){
        try {
            // Send the JSON packet to the client
        echo_server.send(i, val.toStyledString().c_str(), websocketpp::frame::opcode::text);
        } catch (const websocketpp::exception& e) {
            std::cout << "Send failed because: " << e.what() << std::endl;
        }
    }
}
void service::sendNGTSTick(CTORATstpLev2NGTSTickField* pTick){
    Json::Value val;    
    val["ExchangeID"]=pTick->ExchangeID;
    val["SecurityID"]=pTick->SecurityID;
    val["MainSeq"]=pTick->MainSeq;
    val["SubSeq"]=pTick->SubSeq;
    val["TickTime"]=pTick->TickTime;
    val["TickType"]=pTick->TickType;
    val["BuyNo"]=pTick->BuyNo;
    val["SellNo"]=pTick->SellNo;
    val["Price"]=pTick->Price;
    val["Volume"]=pTick->Volume;
    val["TradeMoney"]=pTick->TradeMoney;
    val["Side"]=pTick->Side;
    val["TradeBSFlag"]=pTick->TradeBSFlag;
    val["MDSecurityStat"]=pTick->MDSecurityStat;
    val["Info1"]=pTick->Info1;
    val["Info2"]=pTick->Info2;
    val["Info3"]=pTick->Info3;
}

      