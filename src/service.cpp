#include "../include/service.hpp"
    std::string service::timestampToString(std::time_t timestamp) {
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
            echo_server.send(hdl, response, websocketpp::frame::opcode::text);
        } catch (const websocketpp::exception& e) {
            std::cout << "Send failed because: " << e.what() << std::endl;
        }
    }
    // Callback function when a new connection is established
    void service::on_open( websocketpp::connection_hdl hdl) {
        std::cout << "New connection established" << std::endl;
        socked_V.push_back(hdl);
    }
    void service::on_close( websocketpp::connection_hdl hdl) {
        for(auto it = socked_V.begin(); it != socked_V.end(); ++it) {
            if((*it).lock().get() ==hdl.lock().get()) {
                socked_V.erase(it);
                break;
            }
        }
    }
    void  service::service_init(){
        std::cout<<"service_init"<<std::endl;
        try {
            echo_server.set_access_channels(websocketpp::log::alevel::all);
            echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);
            echo_server.init_asio();
            echo_server.set_message_handler(bind(&service::on_message, this, _1, _2));
            echo_server.set_open_handler(bind(&service::on_open, this, _1));
            echo_server.set_close_handler(bind(&service::on_close, this, _1));
            echo_server.listen(9002);
            echo_server.start_accept();
            echo_server.run();
        } catch (const websocketpp::exception& e) {
            std::cout << e.what() << std::endl;
        } catch (...) {
            std::cout << "Other exception" << std::endl;
        }
        //std::cout<<"service success"<<std::endl;
    }
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
    //std::cout<<val["TradeTime"]<<std::endl;
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
    val["OrderTime"]=timestampToString(pOrderDetail->OrderTime);
    val["Price"]=pOrderDetail->Price;
    val["Volume"]=std::to_string(pOrderDetail->Volume);
    val["Side"]=pOrderDetail->Side;
    val["MainSeq"]=pOrderDetail->MainSeq;
    val["SubSeq"]=pOrderDetail->SubSeq;
    val["Info1"]=pOrderDetail->Info1;
    val["Info2"]=pOrderDetail->Info2;
    val["Info3"]=pOrderDetail->Info3;
    val["OrderNO"]=std::to_string(pOrderDetail->OrderNO);
    val["OrderStatus"]=pOrderDetail->OrderStatus;
    val["BizIndex"]=std::to_string(pOrderDetail->BizIndex);
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
    val["SubSeq"]=std::to_string(pTick->SubSeq);
    val["TickTime"]=timestampToString(pTick->TickTime);
    val["TickType"]=pTick->TickType;
    val["BuyNo"]=std::to_string(pTick->BuyNo);
    val["SellNo"]=std::to_string(pTick->SellNo);
    val["Price"]=pTick->Price;
    val["Volume"]=std::to_string(pTick->Volume);
    val["TradeMoney"]=pTick->TradeMoney;
    val["Side"]=pTick->Side;
    val["TradeBSFlag"]=pTick->TradeBSFlag;
    val["MDSecurityStat"]=pTick->MDSecurityStat;
    val["Info1"]=pTick->Info1;
    val["Info2"]=pTick->Info2;
    val["Info3"]=pTick->Info3;
    for(auto i:socked_V){
        try {
            // Send the JSON packet to the client
        echo_server.send(i, val.toStyledString().c_str(), websocketpp::frame::opcode::text);
        } catch (const websocketpp::exception& e) {
            std::cout << "Send failed because: " << e.what() << std::endl;
        }
    }
}
void service::sendMarketData(CTORATstpLev2MarketDataField *pDepthMarketData){
    Json::Value val;
        val["Ask10NumOrders"]=pDepthMarketData->Ask10NumOrders;
        val["Ask1NumOrders"]=pDepthMarketData->Ask1NumOrders;
        val["Ask2NumOrders"]=pDepthMarketData->Ask2NumOrders;
        val["Ask3NumOrders"]=pDepthMarketData->Ask3NumOrders;
        val["Ask4NumOrders"]=pDepthMarketData->Ask4NumOrders;
        val["Ask5NumOrders"]=pDepthMarketData->Ask5NumOrders;
        val["Ask6NumOrders"]=pDepthMarketData->Ask6NumOrders;
        val["Ask7NumOrders"]=pDepthMarketData->Ask7NumOrders;
        val["Ask8NumOrders"]=pDepthMarketData->Ask8NumOrders;
        val["Ask9NumOrders"]=pDepthMarketData->Ask9NumOrders;
        val["AskPrice10"]=pDepthMarketData->AskPrice10;
        val["AskPrice1"]=pDepthMarketData->AskPrice1;
        val["AskPrice2"]=pDepthMarketData->AskPrice2;
        val["AskPrice3"]=pDepthMarketData->AskPrice3;
        val["AskPrice4"]=pDepthMarketData->AskPrice4;
        val["AskPrice5"]=pDepthMarketData->AskPrice5;
        val["AskPrice6"]=pDepthMarketData->AskPrice6;
        val["AskPrice7"]=pDepthMarketData->AskPrice7;
        val["AskPrice8"]=pDepthMarketData->AskPrice8;
        val["AskPrice9"]=pDepthMarketData->AskPrice9;
        val["AskVolume10"]=std::to_string(pDepthMarketData->AskVolume10);
        val["AskVolume1"]=std::to_string(pDepthMarketData->AskVolume1);
        val["AskVolume2"]=std::to_string(pDepthMarketData->AskVolume2);
        val["AskVolume3"]=std::to_string(pDepthMarketData->AskVolume3);
        val["AskVolume4"]=std::to_string(pDepthMarketData->AskVolume4);
        val["AskVolume5"]=std::to_string(pDepthMarketData->AskVolume5);
        val["AskVolume6"]=std::to_string(pDepthMarketData->AskVolume6);
        val["AskVolume7"]=std::to_string(pDepthMarketData->AskVolume7);
        val["AskVolume8"]=std::to_string(pDepthMarketData->AskVolume8);
        val["AskVolume9"]=std::to_string(pDepthMarketData->AskVolume9);
        val["AvgAskPrice"]=pDepthMarketData->AvgAskPrice;
        val["AvgBidPrice"]=pDepthMarketData->AvgBidPrice;
        val["Bid10NumOrders"]=pDepthMarketData->Bid10NumOrders;
        val["Bid1NumOrders"]=pDepthMarketData->Bid1NumOrders;
        val["Bid2NumOrders"]=pDepthMarketData->Bid2NumOrders;
        val["Bid3NumOrders"]=pDepthMarketData->Bid3NumOrders;
        val["Bid4NumOrders"]=pDepthMarketData->Bid4NumOrders;
        val["Bid5NumOrders"]=pDepthMarketData->Bid5NumOrders;
        val["Bid6NumOrders"]=pDepthMarketData->Bid6NumOrders;
        val["Bid7NumOrders"]=pDepthMarketData->Bid7NumOrders;
        val["Bid8NumOrders"]=pDepthMarketData->Bid8NumOrders;
        val["Bid9NumOrders"]=pDepthMarketData->Bid9NumOrders;
        val["BidPrice10"]=pDepthMarketData->BidPrice10;
        val["BidPrice1"]=pDepthMarketData->BidPrice1;
        val["BidPrice2"]=pDepthMarketData->BidPrice2;
        val["BidPrice3"]=pDepthMarketData->BidPrice3;
        val["BidPrice4"]=pDepthMarketData->BidPrice4;
        val["BidPrice5"]=pDepthMarketData->BidPrice5;
        val["BidPrice6"]=pDepthMarketData->BidPrice6;
        val["BidPrice7"]=pDepthMarketData->BidPrice7;
        val["BidPrice8"]=pDepthMarketData->BidPrice8;
        val["BidPrice9"]=pDepthMarketData->BidPrice9;
        val["BidTradeMaxDuration"]=pDepthMarketData->BidTradeMaxDuration;
        val["BidVolume10"]=std::to_string(pDepthMarketData->BidVolume10);
        val["BidVolume9"]=std::to_string(pDepthMarketData->BidVolume9);
        val["BidVolume8"]=std::to_string(pDepthMarketData->BidVolume8);
        val["BidVolume7"]=std::to_string(pDepthMarketData->BidVolume7);
        val["BidVolume6"]=std::to_string(pDepthMarketData->BidVolume6);
        val["BidVolume5"]=std::to_string(pDepthMarketData->BidVolume5);
        val["BidVolume4"]=std::to_string(pDepthMarketData->BidVolume4);
        val["BidVolume3"]=std::to_string(pDepthMarketData->BidVolume3);
        val["BidVolume2"]=std::to_string(pDepthMarketData->BidVolume2);
        val["BidVolume1"]=std::to_string(pDepthMarketData->BidVolume1);
        val["ClosePrice"]=pDepthMarketData->ClosePrice;
        val["DataTimeStamp"]=pDepthMarketData->DataTimeStamp;
        val["ExchangeID"]=pDepthMarketData->ExchangeID;
        val["HighestPrice"]=pDepthMarketData->HighestPrice;
        val["Info1"]=pDepthMarketData->Info1;
        val["Info2"]=pDepthMarketData->Info2;
        val["Info3"]=pDepthMarketData->Info3;
        val["IOPV"]=pDepthMarketData->IOPV;
        val["LastPrice"]=pDepthMarketData->LastPrice;
        val["LowerLimitPrice"]=pDepthMarketData->LowerLimitPrice;
        val["LowestPrice"]=pDepthMarketData->LowestPrice;
        val["MDSecurityStat"]=pDepthMarketData->MDSecurityStat;
        val["NumTrades"]=std::to_string(pDepthMarketData->NumTrades);
        val["OfferTradeMaxDuration"]=pDepthMarketData->OfferTradeMaxDuration;
        val["OpenPrice"]=pDepthMarketData->OpenPrice;
        val["PreClosePrice"]=pDepthMarketData->PreClosePrice;
        val["SecurityID"]=pDepthMarketData->SecurityID;
        val["UpperLimitPrice"]=pDepthMarketData->UpperLimitPrice;
        val["TotalAskVolume"]=std::to_string(pDepthMarketData->TotalAskVolume);
        val["TotalBidNumber"]=pDepthMarketData->TotalBidNumber;
        val["TotalBidVolume"]=std::to_string(pDepthMarketData->TotalBidVolume);
        val["TotalOfferNumber"]=pDepthMarketData->TotalOfferNumber;
        val["TotalValueTrade"]=pDepthMarketData->TotalValueTrade;
        val["TotalVolumeTrade"]=std::to_string(pDepthMarketData->TotalVolumeTrade);
        val["WithdrawBuyAmount"]=std::to_string(pDepthMarketData->WithdrawBuyAmount);
        val["WithdrawBuyMoney"]=pDepthMarketData->WithdrawBuyMoney;
        val["WithdrawBuyNumber"]=pDepthMarketData->WithdrawBuyNumber;
        val["WithdrawSellAmount"]=std::to_string(pDepthMarketData->WithdrawSellAmount);
        val["WithdrawSellMoney"]=pDepthMarketData->WithdrawSellMoney;
        val["WithdrawSellNumber"]=pDepthMarketData->WithdrawSellNumber;
     for(auto i:socked_V){
            try {
                // Send the JSON packet to the client
            echo_server.send(i, val.toStyledString().c_str(), websocketpp::frame::opcode::text);
            } catch (const websocketpp::exception& e) {
                std::cout << "Send failed because: " << e.what() << std::endl;
            }
        }
}
      