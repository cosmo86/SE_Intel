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
#include "TORATstpLev2ApiStruct.h"
#include "TORATstpLev2ApiDataType.h"
#include "TORATstpLev2MdApi.h"
#include "service.hpp"
#include <openssl/sha.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <ctime>
#include "async_logger.hpp"//???
#include <vector>
#include <thread>
#include <mutex>
#include <list>
#include "jemalloc.h"
#include "concurrentqueue.h"
using namespace TORALEV2API;
typedef websocketpp::server<websocketpp::config::asio> server;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

class Lev2MdSpi : public CTORATstpLev2MdSpi{
public:
	Lev2MdSpi():CH(),SV(),m_api(nullptr),m_request_id(0){};
    Lev2MdSpi(CTORATstpLev2MdApi *api):m_api(api),m_request_id(0){};
    ~Lev2MdSpi(){    logger->stop();};
public:
	virtual void init(char * userid,char * password,char * address);
	virtual	void Service();
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
	void manage_CH();
	void manage_MarketDate();
	void manage_NGTSTick();
	void manage_Transaction();
	void manage_OrderDetail();
	void add(std::string s);
	void Add();
	
	void recive_(void*ptr);//当收到数据时进行处理的回调函数。
    void addstrategy(inputstrategy*ptr);//增加策略
    void getstrategy();//获取策略
    void changestrategy(strategy*ptr);//修改策略
    void removestrategy(json data);//删除策略
	      
private:
	int m_request_id;
    CTORATstpLev2MdApi *m_api;
	char  userid[21];
	char  password[41];
	char  address[64];
    std::list<strategy>Strategy;//策略集合
	int Strategy_id=0;
	ClickHouse CH;
	service SV;
	LoggerPtr logger;
	moodycamel::ConcurrentQueue<TORALEV2API::CTORATstpLev2MarketDataField*> MarketData;
	moodycamel::ConcurrentQueue<TORALEV2API::CTORATstpLev2NGTSTickField*> NGTSTick;
	moodycamel::ConcurrentQueue<TORALEV2API::CTORATstpLev2TransactionField*> Transaction;
	moodycamel::ConcurrentQueue<TORALEV2API::CTORATstpLev2OrderDetailField*> OrderDetail;
};
#endif