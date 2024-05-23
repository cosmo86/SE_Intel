#include "../include/service.hpp"
    void service::sendaddstrategy(strategy*ptr){
         json data;
        data["type"]="Sendinput";
         strategy temporary_data=*ptr;
            data["data"]["BuyTriggerVolume"]=temporary_data.BuyTriggerVolume;
            data["data"]["SecurityID"]= temporary_data.SecurityID;
            data["data"]["ExchangeID"]=temporary_data.ExchangeID;
            data["data"]["OrderID"] =temporary_data.OrderID;
            data["data"]["SecurityName"]=temporary_data.SecurityName;
            data["data"]["ID"]=temporary_data.ID;
           data["data"]["CancelVolume"]= temporary_data.CancelVolume;
            data["data"]["Position"]=temporary_data.Position;
            data["data"]["TargetPosition"]=temporary_data.TargetPosition;
            data["data"]["CurrPosition"]=temporary_data.CurrPosition;
            data["data"]["LowerTimeLimit"]=temporary_data. LowerTimeLimit;
            data["data"]["MaxTriggerTimes"]=temporary_data. MaxTriggerTimes;
            data["data"]["Status"]=temporary_data. Status;
           data["data"]["Count"]= temporary_data. Count;
           data["data"]["ScoutStatus"]=temporary_data. ScoutStatus;
            data["data"]["ScoutBuyTriggerCashLim"]=temporary_data. ScoutBuyTriggerCashLim;
            data["data"]["ScoutMonitorDuration"]=temporary_data. ScoutMonitorDuration;
           data["data"]["Cond2Percent"] =temporary_data. Cond2Percent;
           data["data"]["Cond2HighTime"] =temporary_data. Cond2HighTime;
            data["data"]["Cond2TrackDuration"]=temporary_data. Cond2TrackDuration;
           data["data"]["CancelTriggerVolumeLarge"] =temporary_data. CancelTriggerVolumeLarge;
           data["data"]["Cond4LowTime"] =temporary_data. Cond4LowTime;
            data["data"]["Cond4HighTime"]=temporary_data. Cond4HighTime;
        for(auto i:socked_V){
        try {
            // Send the JSON packet to the client
           echo_server.send(i, data.dump().c_str(), websocketpp::frame::opcode::text);
        } catch (const websocketpp::exception& e) {
            std::cout << "Send failed because: " << e.what() << std::endl;
        }
        }
    }
    void service::sendgetstrategy(std::list<strategy>&val){
        json data;
        data["type"]="getdata";
        int n=val.size();
        data["data"]["sum"]=n;
        std::list<strategy>::iterator it = val.begin();
        for(int i=0;i<n;i++){
            strategy temporary_data=*it;
            data["data"][std::to_string(i)]["BuyTriggerVolume"]=temporary_data.BuyTriggerVolume;
            data["data"][std::to_string(i)]["SecurityID"]= temporary_data.SecurityID;
            data["data"][std::to_string(i)]["ExchangeID"]=temporary_data.ExchangeID;
            data["data"][std::to_string(i)]["OrderID"] =temporary_data.OrderID;
            data["data"][std::to_string(i)]["SecurityName"]=temporary_data.SecurityName;
            data["data"][std::to_string(i)]["ID"]=temporary_data.ID;
           data["data"][std::to_string(i)]["CancelVolume"]= temporary_data.CancelVolume;
            data["data"][std::to_string(i)]["Position"]=temporary_data.Position;
            data["data"][std::to_string(i)]["TargetPosition"]=temporary_data.TargetPosition;
            data["data"][std::to_string(i)]["CurrPosition"]=temporary_data.CurrPosition;
            data["data"][std::to_string(i)]["LowerTimeLimit"]=temporary_data. LowerTimeLimit;
            data["data"][std::to_string(i)]["MaxTriggerTimes"]=temporary_data. MaxTriggerTimes;
            data["data"][std::to_string(i)]["Status"]=temporary_data. Status;
           data["data"][std::to_string(i)]["Count"]= temporary_data. Count;
           data["data"][std::to_string(i)]["ScoutStatus"]=temporary_data. ScoutStatus;
            data["data"][std::to_string(i)]["ScoutBuyTriggerCashLim"]=temporary_data. ScoutBuyTriggerCashLim;
            data["data"][std::to_string(i)]["ScoutMonitorDuration"]=temporary_data. ScoutMonitorDuration;
           data["data"][std::to_string(i)]["Cond2Percent"] =temporary_data. Cond2Percent;
           data["data"][std::to_string(i)]["Cond2HighTime"] =temporary_data. Cond2HighTime;
            data["data"][std::to_string(i)]["Cond2TrackDuration"]=temporary_data. Cond2TrackDuration;
           data["data"][std::to_string(i)]["CancelTriggerVolumeLarge"] =temporary_data. CancelTriggerVolumeLarge;
           data["data"][std::to_string(i)]["Cond4LowTime"] =temporary_data. Cond4LowTime;
            data["data"][std::to_string(i)]["Cond4HighTime"]=temporary_data. Cond4HighTime;
            it++;
        }
        std::cout<<data<<std::endl;
            for(auto i:socked_V){
        try {
            // Send the JSON packet to the client
           echo_server.send(i, data.dump().c_str(), websocketpp::frame::opcode::text);
        } catch (const websocketpp::exception& e) {
            std::cout << "Send failed because: " << e.what() << std::endl;
        }
        }
    }
    void service::sendchangestrategy(strategy*ptr){

    }
    void service::sendremovestrategy(strategy*ptr){

    }
    std::string service::timestampToString(std::time_t timestamp) {
            std::time_t currentTime = std::time(nullptr);
            std::tm* tm_info = std::localtime(&currentTime);
            std::ostringstream oss;
            oss << std::put_time(tm_info, "%Y-%m-%dT%H:%M:%S");
            return oss.str();
        }
    void service::on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
        json data=json::parse(msg->get_payload());
        json *data_ptr=&data;
        fn((void*)(data_ptr));
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
    void  service::service_init(std::function<void(void*)>kn){
        try {
            //echo_server.set_access_channels(websocketpp::log::alevel::all);
            fn=kn;
            echo_server.clear_access_channels(websocketpp::log::alevel::all);
            echo_server.init_asio();
            echo_server.set_open_handler(bind(&service::on_open, this, _1));
            echo_server.set_message_handler(bind(&service::on_message, this, _1, _2));
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
    json val;
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
           echo_server.send(i, val.dump().c_str(), websocketpp::frame::opcode::text);
        } catch (const websocketpp::exception& e) {
            std::cout << "Send failed because: " << e.what() << std::endl;
        }
    }
    //ws://91.208.73.166:9002
}
void service::sendOrderDetail(CTORATstpLev2OrderDetailField* pOrderDetail){
    json val;
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
        echo_server.send(i, val.dump().c_str(), websocketpp::frame::opcode::text);
        } catch (const websocketpp::exception& e) {
            std::cout << "Send failed because: " << e.what() << std::endl;
        }
    }
}
void service::sendNGTSTick(CTORATstpLev2NGTSTickField* pTick){
    json val;    
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
        echo_server.send(i, val.dump().c_str(), websocketpp::frame::opcode::text);
        } catch (const websocketpp::exception& e) {
            std::cout << "Send failed because: " << e.what() << std::endl;
        }
    }
}
void service::sendMarketData(CTORATstpLev2MarketDataField *pDepthMarketData){
    json val;
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
            echo_server.send(i, val.dump().c_str(), websocketpp::frame::opcode::text);
            } catch (const websocketpp::exception& e) {
                std::cout << "Send failed because: " << e.what() << std::endl;
            }
        }
}
      