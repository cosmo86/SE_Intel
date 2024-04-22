#ifndef CLICKHOUSE
#define CLICKHOUSE
#include <clickhouse/client.h>
#include <string>
#include <sstream>
#include <vector>
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
    public:
        ClickHouse():client(ClientOptions().SetHost("localhost")){}
        ~ClickHouse(){}
    public:
        void clickhouse_init(){}
        void buildMarketData();
        void insertMarketData(CTORATstpLev2MarketDataField *pMarketData);
        void buildNGTSTick();
        void insertNGTSTick(CTORATstpLev2NGTSTickField* pTick);
        void buildTransaction();
        void insertTransaction(CTORATstpLev2TransactionField* pTransaction);
        void buildOrderDetail();
        void insertOrderDetail(CTORATstpLev2OrderDetailField* pOrderDetail);
        void execute(std::string sql);
};
#endif