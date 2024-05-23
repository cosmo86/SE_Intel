#ifndef SERVICE_HPP
#define SERVICE_HPP
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <string>
#include <vector>
#include "TORATstpLev2ApiStruct.h"
#include "TORATstpLev2ApiDataType.h"
#include "TORATstpLev2MdApi.h"
#include <nlohmann/json.hpp>
#include <functional>
#include <list>
// for convenience
using json = nlohmann::json;
using namespace TORALEV2API;
using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
typedef websocketpp::server<websocketpp::config::asio> server;
enum Send_name{
    Sendinput=0//创建新策略
    ,getdata//请求现有策略
    ,Sendchange//改变指定策略
    ,Senddelete//删除指定策略
};
struct strategy{
    std::string SecurityID;//证券代码
    std::string ExchangeID;//交易所
    std::string OrderID;//策略委托
    std::string SecurityName;//股票名称
    std::string ID;//策略编号
    int BuyTriggerVolume;//封单额
    int CancelVolume;//撤单额
    int Position;//目标仓位 元
    int TargetPosition;//目标仓位 股
    int CurrPosition;//已买仓位 股
    int LowerTimeLimit;//延迟触发
    int MaxTriggerTimes;//大单延迟时间
    int Status;//策略状态
    int Count;//撤单次数 nullptr
    int ScoutStatus;//保护单状态 nullptr
    int ScoutBuyTriggerCashLim;//保护单触发金额 nullptr
    int ScoutMonitorDuration;//保护单监控区间 nullptr
    int Cond2Percent;//撤单动量比例 nullptr
    int Cond2HighTime;//撤单动量监控时间 nullptr
    int Cond2TrackDuration;//撤单动量时间区间 nullptr
    int CancelTriggerVolumeLarge;//大单大撤单金额 nullptr
    int Cond4LowTime;//大单大撤单起始时间 nullptr
    int Cond4HighTime;//大单大撤单结束时间 nullptr
};
struct inputstrategy{
    std::string SecurityID;//证券代码
    std::string ExchangeID;//交易所
    int BuyTriggerVolume;//封单额
    int CancelVolume;//撤单额
    int DBVolume;//打板金额
    int ordercancellationcount;//撤单金额
    int MaxTriggerTimes;//大单延迟时间
    int ScoutBuyTriggerCashLim;//保护单触发金额 nullptr
    int minMonitorTimes;//小单监控时间
    int Cond2Percent;//撤单动量比例 nullptr
    int Cond2HighTime;//撤单动量监控时间 nullptr
    int Cond2TrackDuration;//撤单动量时间区间 nullptr
    int  MaxBuyTriggerVolume;//大单大封单金额
};
class service
{
private:
    server echo_server;
    std::vector<websocketpp::connection_hdl> socked_V;
    static std::string timestampToString(std::time_t timestamp);
    std::function<void(void*)>fn;//可调用对象
public:
    service() : echo_server(), socked_V(),fn() {}
    ~service() {}

public:
    void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg);
    void on_open(websocketpp::connection_hdl hdl);
    void on_close(websocketpp::connection_hdl hdl);
    void sendTransaction(CTORATstpLev2TransactionField *pTransaction);
    void sendOrderDetail(CTORATstpLev2OrderDetailField *pOrderDetail);
    void sendNGTSTick(CTORATstpLev2NGTSTickField *pTick);
    void sendMarketData(CTORATstpLev2MarketDataField *pMarketData);
    void service_init(std::function<void(void*)>kn);
    void sendaddstrategy(strategy*ptr);//增加策略
    void sendgetstrategy(std::list<strategy>&val);//获取策略
    void sendchangestrategy(strategy*ptr);//修改策略
    void sendremovestrategy(strategy*ptr);//删除策略
};

#endif