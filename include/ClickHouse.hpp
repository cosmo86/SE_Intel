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
        std::unordered_map<std::string, std::function<void()>> buildFunctions;
        static std::string getCurrentDate();
        void buildNGTSTick();
        void buildTransaction();
        void buildOrderDetail();
    public:
        bool Init_CH();
        ClickHouse():client(ClientOptions().SetHost("localhost")){}
        ~ClickHouse(){}
    public:
        void execute(std::string sql);
        bool build(std::string sign);
        void Insert(std::vector<void *>&it,int sign){}
};
#endif