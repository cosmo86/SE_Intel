#ifndef CLICKHOUSE
#define CLICKHOUSE
#include <clickhouse/client.h>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <functional>
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
        void buildNGTSTick();
        void insertNGTSTick();
        void buildTransaction();
        void insertTransaction();
        void buildOrderDetail();
        void insertOrderDetail();
        void execute(std::string sql);
        bool build(std::string sign);
        void Insert(std::vector<void *>&it,int sign){}
};
#endif