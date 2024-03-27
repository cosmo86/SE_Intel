/***********************************************************************
*	@history	2024-03-27
*	@author		ZHANGXUAN
*   @contact    Earth
***********************************************************************/
#ifndef L2_QUOTER_HPP
#define L2_QUOTER_HPP
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
//#include <map>
#include "TORATstpLev2MdApi.h"
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
using namespace TORALEV2API;
class Lev2MdSpi : public CTORATstpLev2MdSpi{
public:
    Lev2MdSpi(CTORATstpLev2MdApi *api):m_api(api),m_request_id(0){};
    ~Lev2MdSpi(){};
public:
	//virtual void connect(const char* const userid, const char* const password, const char* const address, const char* const mode);//用户ID、密码、服务器地址和连接模式
	//request
	virtual void OnFrontConnected(std::string username,std::string password);
	virtual void OnRspUserLogin(CTORATstpRspUserLoginField *pRspUserLogin, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//virtual void Stop();
	//virtual void OnFrontDisconnected(int nReason);
	/// Error response
	//virtual void OnRspError(TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	/// Login request response，登陆响应
    // Subscribe 
	/*virtual void Subscribe( char* ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID);
    //UnSubscribe
	virtual void UnSubscribe(char* ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID);
	********************************** Return Data Callback Function**********************************
	/// Logout
	virtual void OnRspUserLogout(TORALEV2API::CTORATstpUserLogoutField* pUserLogout, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	/// Subscribe NGTSTICK
	virtual void OnRspSubNGTSTick(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	*//// OnRspSubMarketData
	virtual void OnRspSubMarketData(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	/// OnRspUnSubMarketData
	/*virtual void OnRspUnSubMarketData(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	// OnRspSubTransaction
	virtual void OnRspSubTransaction(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	/// OnRspSubOrderDetail
	virtual void OnRspSubOrderDetail(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	/*
    ********************************** Data call back **********************************
	/// OnRtnMarketData
    */
	virtual void OnRtnMarketData(CTORATstpLev2MarketDataField* pDepthMarketData, const int FirstLevelBuyNum, const int FirstLevelBuyOrderVolumes[], const int FirstLevelSellNum, const int FirstLevelSellOrderVolumes[]);
	void add(){
        char * security_list[]={"510050","600***","688***"};
        int ret = m_api->SubscribeMarketData (security_list,3, TORA_TSTP_EXD_SSE);
        if (ret != 0){
            printf("SubscribeMarketData fail, ret[%d]\n", ret);
        }
    }
    //NGT ֪ͨ
	/*virtual void OnRtnNGTSTick(CTORATstpLev2NGTSTickField* pTick);
	/// OnRtnTransaction
	virtual void OnRtnTransaction(CTORATstpLev2TransactionField* pTransaction);
	/// OnRtnOrderDetail
	virtual void OnRtnOrderDetail(CTORATstpLev2OrderDetailField* pOrderDetail);*/

private:
	int m_request_id;
    CTORATstpLev2MdApi *m_api;
	//CTORATstpLev2MdApi* m_api = NULL;
	//int m_req_id;
	//char m_userid[21];
	//char m_password[41];
	//char m_address[64];
	//char m_mode[21];
	//moodycamel::ConcurrentQueue<SEEvent>* m_Event_Q_ptr = nullptr;
	//moodycamel::ConcurrentQueue<std::shared_ptr<SEObject>>* m_Event_Q_ptr;
	//LoggerPtr m_logger=nullptr;
	//std::map<int,std::tuple<Strategy*,bool>>* m_StrategyMap_Ptr;
};

#endif