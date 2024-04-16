/***********************************************************************
*	@history	2024-03-27
*	@author		ZHANGXUAN
*   @contact    Earth
***********************************************************************/
#ifndef L2_QUOTER_HPP
#define L2_QUOTER_HPP
#pragma once
/*#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <iomanip> // 用于格式化输出
#include <sstream> // 用于字符串流
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <event.h>
#include <event2/event.h>
//#include <map>
#include <openssl/sha.h>
#include "TORATstpLev2MdApi.h"
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
//#include </root/clickhouse-cpp/clickhouse/client.h>
//#include "/root/clickhouse-cpp/clickhouse/client.h"//数据库
#include <clickhouse/client.h>
#include <unistd.h>
#include <ctime>
//#include <json/json.h>
#include "/root/vcpkg/packages/jsoncpp_x64-linux/include/json/json.h"*/
//#include "concurrentqueue.h"
//#include <jemalloc/jemalloc.h>
//#include "Strategy.hpp"
//#include "Lv2dataModel.hpp"
//#include "Event.h"
//#include "SEObject.hpp"
//#include "spdlog/spdlog.h"
//#include "spdlog/sinks/stdout_color_sinks.h"
//#include "spdlog/sinks/basic_file_sink.h"
//#include "spdlog/async.h"
//#include "helper_functions.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <event.h>
#include <event2/event.h>
#include <openssl/sha.h>
#include "TORATstpLev2MdApi.h"
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <clickhouse/client.h>
#include <unistd.h>
#include <ctime>
#include "/root/vcpkg/packages/jsoncpp_x64-linux/include/json/json.h"
#include <vector>
#include <ctime>
//#include <map>
//#include </root/clickhouse-cpp/clickhouse/client.h>
//#include "/root/clickhouse-cpp/clickhouse/client.h"  // 数据库
//#include <json/json.h>
using namespace TORALEV2API;
using namespace clickhouse;//数据库下的名字空间
typedef websocketpp::server<websocketpp::config::asio> server;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
class test{
private:
	int a;
	double b;
	std::string c;
public:
	test():a(0),b(0),c(""){}
	test(int a1,double b1,std::string c1):a(a1),b(b1),c(c1){}
	~test(){}
	void print(){
		std::cout<<"type=1"<<std::endl;
	}
	void print(test *p);
	void print_zidingyi();
};
class Lev2MdSpi : public CTORATstpLev2MdSpi{
public:
	Lev2MdSpi():m_api(nullptr),m_request_id(0){};
    Lev2MdSpi(CTORATstpLev2MdApi *api):m_api(api),m_request_id(0){};
    ~Lev2MdSpi(){};
public:
	std::string gettime();
	virtual void init(char * userid,char * password,char * address);
	virtual void OnFrontConnected();
	virtual void OnRspUserLogin(CTORATstpRspUserLoginField *pRspUserLogin, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//virtual void OnRspUserLogout(TORALEV2API::CTORATstpUserLogoutField* pUserLogout, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	//virtual void OnRspSubNGTSTick(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	
	virtual void OnRspSubNGTSTick(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	//virtual void OnRspSubMarketData(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspSubTransaction(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspSubOrderDetail(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);



	//virtual void OnRtnMarketData(CTORATstpLev2MarketDataField* pDepthMarketData, const int FirstLevelBuyNum, const int FirstLevelBuyOrderVolumes[], const int FirstLevelSellNum, const int FirstLevelSellOrderVolumes[]);
	virtual void OnRtnNGTSTick(CTORATstpLev2NGTSTickField* pTick);
	virtual void OnRtnTransaction(CTORATstpLev2TransactionField* pTransaction);
	virtual void OnRtnOrderDetail(CTORATstpLev2OrderDetailField* pOrderDetail);
	void add();
private:
	int m_request_id;
    CTORATstpLev2MdApi *m_api;
	char  userid[21];
	char  password[41];
	char  address[64];
};
#endif