/***********************************************************************
*	@history	2024-03-27
*	@author		ZHANGXUAN
*   @contact    Earth
***********************************************************************/
/*#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>*/
/*#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <event.h>
#include <event2/event.h>*/
#ifndef L2_QUOTER_HPP
#define L2_QUOTER_HPP
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "ClickHouse.hpp"
#include "TORATstpLev2MdApi.h"
#include "service.hpp"
#include <openssl/sha.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <ctime>
#include "/root/vcpkg/packages/jsoncpp_x64-linux/include/json/json.h"
#include <vector>
#include <thread>
using namespace TORALEV2API;
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
	Lev2MdSpi():CH(),SV(),m_api(nullptr),m_request_id(0){};
    Lev2MdSpi(CTORATstpLev2MdApi *api):m_api(api),m_request_id(0){};
    ~Lev2MdSpi(){};
public:
	virtual void init(char * userid,char * password,char * address);
	virtual	void init_CH_SV();
	virtual void OnFrontConnected();
	virtual void OnRspUserLogin(CTORATstpRspUserLoginField *pRspUserLogin, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspSubMarketData (CTORATstpSpecificSecurityField*pSpecificSecurity, CTORATstpRspInfoField*pRspInfo, int nRequestID, bool bIsLast) ;
	virtual void OnRspSubNGTSTick(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);	
	virtual void OnRspSubTransaction(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspSubOrderDetail(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnMarketData(CTORATstpLev2MarketDataField* pDepthMarketData, const int FirstLevelBuyNum, const int FirstLevelBuyOrderVolumes[], const int FirstLevelSellNum, const int FirstLevelSellOrderVolumes[]);
	//virtual void OnRtnMarketData (CTORATstpLev2MarketDataField *pMarketData, const int *FirstLevelBuyNum, const int FirstLevelBuyOrderVolumes[],const int *FirstLevelSellNum, const int FirstLevelSellOrderVolumes[]);
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
	ClickHouse CH;
	service SV;
};
#endif