#ifndef CLICKHOUSE
#define CLICKHOUSE
#include <clickhouse/client.h>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <iostream>
#include "TORATstpLev2ApiStruct.h"
#include "TORATstpLev2ApiDataType.h"
#include "TORATstpLev2MdApi.h"

using namespace TORALEV2API;
using namespace clickhouse;//数据库下的名字空间
class ClickHouse{
    private:
        Client client;
        static std::string getCurrentDate();
        std::queue<std::string> transactionQueue;
        std::queue<std::string> orderDetailQueue;
        std::queue<std::string> ngtstickQueue;
        std::queue<std::string> marketDataQueue;
        std::time_t lastMarketDataTime = std::time(nullptr); 
        std::time_t lastNGTSTicTime = std::time(nullptr); 
        std::time_t lastTransactionTime = std::time(nullptr); 
        std::time_t lastOrderDetailTime = std::time(nullptr); 
    public:
        ClickHouse():client(ClientOptions().SetHost("localhost")){}
        ~ClickHouse(){}
    public:
        void clickhouse_init(){}
        void buildMarketData();
        void insertMarketData(CTORATstpLev2MarketDataField *pMarketData);
        void ExcuteMarketData();
        void buildNGTSTick();
        void insertNGTSTick(CTORATstpLev2NGTSTickField* pTick);
        void ExcuteNGTSTick();
        void buildTransaction();
        void insertTransaction(CTORATstpLev2TransactionField* pTransaction);
        void ExcuteTransaction();
        void buildOrderDetail();
        void insertOrderDetail(CTORATstpLev2OrderDetailField* pOrderDetail);
        void ExcuteOrderDetail();
        void execute(std::string sql);
};
#endif