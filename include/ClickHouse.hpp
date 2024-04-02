#ifndef CLICKHOUSE
#define CLICKHOUSE
#include </root/clickhouse-app/contribs/clickhouse-cpp/clickhouse/client.h>
#include <iostream>
#include <string>
using namespace clickhouse;//数据库下的名字空间
using namespace std;
class ClickHouse{//数据库类
private:
    Client client; 
public:
    ClickHouse(string client_ip="localhost"):client(ClientOptions().SetHost(client_ip.c_str())){}
    ~ClickHouse(){}
public:
    void Init(); //初始化
};




#endif