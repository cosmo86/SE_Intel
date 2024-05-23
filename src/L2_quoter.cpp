#include "../include/L2_quoter.hpp"//头文件信息
using namespace std;
	//初始化，账号，密码，连接地址,数据库，网络
    void Lev2MdSpi::init(char * userid,char * password,char * address){
        strcpy(this->userid,userid);//账号
        strcpy(this->password,password);//密码
        strcpy(this->address,address);//地址
        m_api=CTORATstpLev2MdApi::CreateTstpLev2MdApi(TORA_TSTP_MST_TCP,true);
        m_api->RegisterFront(address);//调用 RegisterFront 方法注册前置机地址
        m_api->RegisterSpi(this);//派生自回调接口类的实例
        m_api->Init();
        logger=GetLogger();
        logger->info("Init yes");
    }
    //初始化网络和数据库，并接受数据库数据
    void Lev2MdSpi::Service(){
        //this->SV.service_init(bind(&Lev2MdSpi::recive_,this,_1));//网络初始化
        this->SV.service_init(std::function<void(void*)>(std::bind(&Lev2MdSpi::recive_, this, std::placeholders::_1)));
    }
    void Lev2MdSpi:: recive_(void*it){
        json *pt=(json*)it;
        json data=*pt;
        if(strncmp(data["type"].get<std::string>().c_str(),"Sendinput",9)==0){
            //std::cout<<data<<std::endl;
            inputstrategy*ptr=new inputstrategy();
            ptr->SecurityID=data["data"]["SecurityID"];
            ptr->ExchangeID=data["data"]["ExchangeID"];
            ptr-> BuyTriggerVolume=data["data"]["BuyTriggerVolume"];
            ptr->CancelVolume=data["data"]["CancelVolume"];
            ptr-> DBVolume=data["data"]["DBVolume"];
            ptr-> ordercancellationcount=data["data"]["ordercancellationcount"];
            ptr-> MaxTriggerTimes=data["data"]["MaxTriggerTimes"];
            ptr-> ScoutBuyTriggerCashLim=data["data"]["ScoutBuyTriggerCashLim"];
            ptr-> minMonitorTimes=data["data"]["minMonitorTimes"];
            ptr-> Cond2Percent=data["data"]["Cond2Percent"];
            ptr-> Cond2HighTime=data["data"]["Cond2HighTime"];
            ptr-> Cond2TrackDuration=data["data"]["Cond2TrackDuration"];
            ptr->  MaxBuyTriggerVolume=data["data"]["MaxBuyTriggerVolume"];
            addstrategy(ptr);
        }
        if(strncmp(data["type"].get<std::string>().c_str(),"getdata",7)==0){
            getstrategy();
        }
        if(strncmp(data["type"].get<std::string>().c_str(),"Senddelete",10)==0){
            //std::cout<<"senddelete"<<std::endl;
            removestrategy(data);
        }
    }
    void  Lev2MdSpi::addstrategy(inputstrategy*ptr){
            strategy data;
                data.SecurityID= ptr->SecurityID;
                data. ExchangeID=ptr->ExchangeID;
                data.OrderID=std::string("OrderID");//策略委托
                data.SecurityName=std::string("SecurityName");//股票名称
                data.ID=std::to_string(++Strategy_id);//策略编号
                data.BuyTriggerVolume=ptr-> BuyTriggerVolume;
                data.CancelVolume=ptr->CancelVolume;
                data.Position=0;//目标仓位 元
                data.TargetPosition=0;//目标仓位 股
                data.CurrPosition=0;//已买仓位 股
                data.LowerTimeLimit=0;//延迟触发
                data.MaxTriggerTimes= ptr-> MaxTriggerTimes;//大单延迟时间
                data.Status=0;//策略状态
                data.Count=0;//撤单次数 nullptr
                data.ScoutStatus=0;//保护单状态 nullptr
                data.ScoutBuyTriggerCashLim=ptr-> ScoutBuyTriggerCashLim;//保护单触发金额 nullptr
                data.ScoutMonitorDuration=0;//保护单监控区间 nullptr
                data.Cond2Percent=ptr-> Cond2Percent;//撤单动量比例 nullptr
                data.Cond2HighTime= ptr-> Cond2HighTime;//撤单动量监控时间 nullptr
                data.Cond2TrackDuration=ptr-> Cond2TrackDuration;//撤单动量时间区间 nullptr
                data.CancelTriggerVolumeLarge=0;//大单大撤单金额 nullptr
                data.Cond4LowTime=0;//大单大撤单起始时间 nullptr
                data.Cond4HighTime=0;//大单大撤单结束时间 nullptr
            Strategy.push_back(data);
            SV.sendaddstrategy(&data);
    }
    void  Lev2MdSpi::getstrategy(){
        SV.sendgetstrategy(Strategy);
    }
    void  Lev2MdSpi::changestrategy(strategy*ptr){

    }
    void  Lev2MdSpi::removestrategy(json data){
        int i=data["data"]["sum"].get<int>();
        vector<std::string>ans;
        for(int j=0;j<i;j++){
            ans.push_back(data["data"][std::to_string(j)]["ID"].get<std::string>());
            std::cout<<ans[j]<<std::endl;
        }
        for (auto it = Strategy.begin(); it != Strategy.end(); ) {
            if (std::find(ans.begin(), ans.end(), (*it).ID) != ans.end()) {
                // 删除当前元素，并继续循环，但不增加迭代器
                it = Strategy.erase(it);
            } else {
                // 如果不是要删除的元素，就移动到下一个元素
                ++it;
            }
        }
        SV.sendgetstrategy(Strategy);


          
    }
    //登陆
    void Lev2MdSpi::OnFrontConnected(){
        logger->info("OnFrontConnected!");
        CTORATstpReqUserLoginField req_user_login_field;
        memset(&req_user_login_field, 0, sizeof(req_user_login_field));//清空
        strcpy(req_user_login_field.LogInAccount, userid);//userid
        req_user_login_field.LogInAccountType =  TORALEV2API::TORA_TSTP_LACT_UserID;//login_type
        strcpy(req_user_login_field.Password, password);//password
        strcpy(req_user_login_field.UserProductInfo, "HX5ZJ0C1PV");//用户端产品信息
        int ret = m_api->ReqUserLogin(&req_user_login_field, ++m_request_id);//调用登陆函数，参数为登陆请求信息
        if (ret != 0)//登陆失败返回0
        {
            logger->warn("ReqUserLogin fail, ret[%d]\n", ret);
        }
    }
    //登陆响应
    void Lev2MdSpi:: OnRspUserLogin(CTORATstpRspUserLoginField *pRspUserLogin, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
        if (pRspInfo->ErrorID == 0){
            logger->info("login success");
        }
        else{
            logger->warn("login fail, error_id[%d] error_msg[%s]\n",pRspInfo ->ErrorID, pRspInfo ->ErrorMsg);
        }
    }
    void Lev2MdSpi::Add(){
        while(1){
            string S;
            cout<<"cin>>";
            cin>>S;
            cout<<S<<endl;
            if(S.length()==6){
                add(S);
            }
        }
    }
    void Lev2MdSpi::add(std::string s){
        cout<<s<<endl;
        char* security_list[1];
        char nonconst_id[31];
        strcpy(nonconst_id ,s.c_str());
        security_list[0] = nonconst_id;
        if (m_api->SubscribeNGTSTick(security_list, sizeof(security_list) / sizeof(char*), TORA_TSTP_EXD_SZSE) == 0){
            logger->info("add SubscribeNGTSTick success!");
        }
        else{
            logger->warn("add SubscribeNGTSTick error!");
        }
		//Subscribe old orderdetial 
		if (m_api->SubscribeOrderDetail(security_list, sizeof(security_list) / sizeof(char*),TORA_TSTP_EXD_SZSE) == 0){
			logger->info("add SubscribeOrderDetail success!");
        }
		else{
			logger->warn("add SubscribeOrderDetail error!");
		}
		//Subscribe old trasaction 
        if (m_api->SubscribeTransaction(security_list, sizeof(security_list) / sizeof(char*), TORA_TSTP_EXD_SZSE) == 0){
			logger->info("add SubscribeTransaction success!");
		}
		else{
			logger->warn("add SubscribeTransaction error!");
		}
        if(m_api->SubscribeMarketData(security_list,sizeof(security_list) / sizeof(char*),TORA_TSTP_EXD_SZSE) ==0){
            logger->info("add SubscribeMarketData success!");
        }
        else{
            logger->warn("add SubscribeMarketData error!");
        }
        
        
    }
    
    //------------------------------------------------------------------------响应报文----------------------------------------------------------------------------------//
    void Lev2MdSpi::OnRspSubNGTSTick(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast){
        if (pRspInfo->ErrorID == 0){
            logger->info("OnRspSubNGTSTick");
            CH.buildNGTSTick();
        }
        else{
            logger->warn("sub market data fail, error_id[%d] error_msg[%s] \n",pRspInfo->ErrorID, pRspInfo->ErrorMsg);
        }
    }
	void Lev2MdSpi::OnRspSubTransaction(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast){
        if (pRspInfo->ErrorID == 0){
            logger->info("OnRspSubTransaction");
            CH.buildTransaction();
        }
        else{
            logger->warn("sub market data fail, error_id[%d] error_msg[%s] \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
        }
    }
	void Lev2MdSpi::OnRspSubOrderDetail(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast){
        
        if (pRspInfo->ErrorID == 0){ 
        logger->info("OnRspSubOrderDetail");
           CH.buildOrderDetail();
        }
        else{
            logger->warn("sub market data fail, error_id[%d] error_msg[%s] \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
        }
    }
    void Lev2MdSpi::OnRspSubMarketData (CTORATstpSpecificSecurityField*pSpecificSecurity, CTORATstpRspInfoField*pRspInfo, int nRequestID, bool bIsLast) {
        if (pRspInfo->ErrorID == 0){
           logger->info("OnRspSubMarketData");
           CH.buildMarketData();
        }
        else{
         logger->warn("sub market data fail, error_id[%d] error_msg[%s] \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
        }
    }

void Lev2MdSpi::OnRtnMarketData(CTORATstpLev2MarketDataField* pDepthMarketData, const int FirstLevelBuyNum, const int FirstLevelBuyOrderVolumes[], const int FirstLevelSellNum, const int FirstLevelSellOrderVolumes[]){
        CTORATstpLev2MarketDataField*ptr=(CTORATstpLev2MarketDataField*)je_malloc(sizeof(CTORATstpLev2MarketDataField));
        if(ptr==nullptr){
        cout<<"malloc err"<<endl;
        exit(0);
        }
        ptr->Ask10NumOrders=pDepthMarketData->Ask10NumOrders;
        ptr->Ask1NumOrders=pDepthMarketData->Ask1NumOrders;
        ptr->Ask2NumOrders=pDepthMarketData->Ask2NumOrders;
        ptr->Ask3NumOrders=pDepthMarketData->Ask3NumOrders;
        ptr->Ask4NumOrders=pDepthMarketData->Ask4NumOrders;
        ptr->Ask5NumOrders=pDepthMarketData->Ask5NumOrders;
        ptr->Ask6NumOrders=pDepthMarketData->Ask6NumOrders;
        ptr->Ask7NumOrders=pDepthMarketData->Ask7NumOrders;
        ptr->Ask8NumOrders=pDepthMarketData->Ask8NumOrders;
        ptr->Ask9NumOrders=pDepthMarketData->Ask9NumOrders;
        ptr->AskPrice10=pDepthMarketData->AskPrice10;
        ptr->AskPrice1=pDepthMarketData->AskPrice1;
        ptr->AskPrice2=pDepthMarketData->AskPrice2;
        ptr->AskPrice3=pDepthMarketData->AskPrice3;
        ptr->AskPrice4=pDepthMarketData->AskPrice4;
        ptr->AskPrice5=pDepthMarketData->AskPrice5;
        ptr->AskPrice6=pDepthMarketData->AskPrice6;
        ptr->AskPrice7=pDepthMarketData->AskPrice7;
        ptr->AskPrice8=pDepthMarketData->AskPrice8;
        ptr->AskPrice9=pDepthMarketData->AskPrice9;
        ptr->AskVolume10=pDepthMarketData->AskVolume10;
        ptr->AskVolume1=pDepthMarketData->AskVolume1;
        ptr->AskVolume2=pDepthMarketData->AskVolume2;
        ptr->AskVolume3=pDepthMarketData->AskVolume3;
        ptr->AskVolume4=pDepthMarketData->AskVolume4;
        ptr->AskVolume5=pDepthMarketData->AskVolume5;
        ptr->AskVolume6=pDepthMarketData->AskVolume6;
        ptr->AskVolume7=pDepthMarketData->AskVolume7;
        ptr->AskVolume8=pDepthMarketData->AskVolume8;
        ptr->AskVolume9=pDepthMarketData->AskVolume9;
        ptr->AvgAskPrice=pDepthMarketData->AvgAskPrice;
        ptr->AvgBidPrice=pDepthMarketData->AvgBidPrice;
        ptr->Bid10NumOrders=pDepthMarketData->Bid10NumOrders;
        ptr->Bid1NumOrders=pDepthMarketData->Bid1NumOrders;
        ptr->Bid2NumOrders=pDepthMarketData->Bid2NumOrders;
        ptr->Bid3NumOrders=pDepthMarketData->Bid3NumOrders;
        ptr->Bid4NumOrders=pDepthMarketData->Bid4NumOrders;
        ptr->Bid5NumOrders=pDepthMarketData->Bid5NumOrders;
        ptr->Bid6NumOrders=pDepthMarketData->Bid6NumOrders;
        ptr->Bid7NumOrders=pDepthMarketData->Bid7NumOrders;
        ptr->Bid8NumOrders=pDepthMarketData->Bid8NumOrders;
        ptr->Bid9NumOrders=pDepthMarketData->Bid9NumOrders;
        ptr->BidPrice10=pDepthMarketData->BidPrice10;
        ptr->BidPrice1=pDepthMarketData->BidPrice1;
        ptr->BidPrice2=pDepthMarketData->BidPrice2;
        ptr->BidPrice3=pDepthMarketData->BidPrice3;
        ptr->BidPrice4=pDepthMarketData->BidPrice4;
        ptr->BidPrice5=pDepthMarketData->BidPrice5;
        ptr->BidPrice6=pDepthMarketData->BidPrice6;
        ptr->BidPrice7=pDepthMarketData->BidPrice7;
        ptr->BidPrice8=pDepthMarketData->BidPrice8;
        ptr->BidPrice9=pDepthMarketData->BidPrice9;
        ptr->BidTradeMaxDuration=pDepthMarketData->BidTradeMaxDuration;
        ptr->BidVolume10=pDepthMarketData->BidVolume10;
        ptr->BidVolume9=pDepthMarketData->BidVolume9;
        ptr->BidVolume8=pDepthMarketData->BidVolume8;
        ptr->BidVolume7=pDepthMarketData->BidVolume7;
        ptr->BidVolume6=pDepthMarketData->BidVolume6;
        ptr->BidVolume5=pDepthMarketData->BidVolume5;
        ptr->BidVolume4=pDepthMarketData->BidVolume4;
        ptr->BidVolume3=pDepthMarketData->BidVolume3;
        ptr->BidVolume2=pDepthMarketData->BidVolume2;
        ptr->BidVolume1=pDepthMarketData->BidVolume1;
        ptr->ClosePrice=pDepthMarketData->ClosePrice;
        ptr->DataTimeStamp=pDepthMarketData->DataTimeStamp;
        ptr->ExchangeID=pDepthMarketData->ExchangeID;
        ptr->HighestPrice=pDepthMarketData->HighestPrice;
        ptr->Info1=pDepthMarketData->Info1;
        ptr->Info2=pDepthMarketData->Info2;
        ptr->Info3=pDepthMarketData->Info3;
        ptr->IOPV=pDepthMarketData->IOPV;
        ptr->LastPrice=pDepthMarketData->LastPrice;
        ptr->LowerLimitPrice=pDepthMarketData->LowerLimitPrice;
        ptr->LowestPrice=pDepthMarketData->LowestPrice;
        ptr->MDSecurityStat=pDepthMarketData->MDSecurityStat;
        ptr->NumTrades=pDepthMarketData->NumTrades;
        ptr->OfferTradeMaxDuration=pDepthMarketData->OfferTradeMaxDuration;
        ptr->OpenPrice=pDepthMarketData->OpenPrice;
        ptr->PreClosePrice=pDepthMarketData->PreClosePrice;
        strcpy(ptr->SecurityID,pDepthMarketData->SecurityID);
        ptr->UpperLimitPrice=pDepthMarketData->UpperLimitPrice;
        ptr->TotalAskVolume=pDepthMarketData->TotalAskVolume;
        ptr->TotalBidNumber=pDepthMarketData->TotalBidNumber;
        ptr->TotalBidVolume=pDepthMarketData->TotalBidVolume;
        ptr->TotalOfferNumber=pDepthMarketData->TotalOfferNumber;
        ptr->TotalValueTrade=pDepthMarketData->TotalValueTrade;
        ptr->TotalVolumeTrade=pDepthMarketData->TotalVolumeTrade;
        ptr->WithdrawBuyAmount=pDepthMarketData->WithdrawBuyAmount;
        ptr->WithdrawBuyMoney=pDepthMarketData->WithdrawBuyMoney;
        ptr->WithdrawBuyNumber=pDepthMarketData->WithdrawBuyNumber;
        ptr->WithdrawSellAmount=pDepthMarketData->WithdrawSellAmount;
        ptr->WithdrawSellMoney=pDepthMarketData->WithdrawSellMoney;
        ptr->WithdrawSellNumber=pDepthMarketData->WithdrawSellNumber;
        MarketData.enqueue(ptr);

    }

void Lev2MdSpi::OnRtnNGTSTick(CTORATstpLev2NGTSTickField* pTick){
  
       CTORATstpLev2NGTSTickField*ptr=(CTORATstpLev2NGTSTickField*)je_malloc(sizeof(CTORATstpLev2NGTSTickField));
       
        if(ptr==nullptr){
        cout<<"malloc err"<<endl;
        exit(0);
        }
         ptr->BuyNo=pTick->BuyNo;
         ptr->ExchangeID=pTick->ExchangeID;
         ptr->Info1=pTick->Info1;
         ptr->Info2=pTick->Info2;
         ptr->Info3=pTick->Info3;
         ptr->MainSeq=pTick->MainSeq;
         ptr->MDSecurityStat=pTick->MDSecurityStat;
         ptr->Price=pTick->Price;
         strcpy(ptr->SecurityID,pTick->SecurityID);
         ptr->SellNo=pTick->SellNo;
         ptr->Side=pTick->Side;
         ptr->SubSeq=pTick->SubSeq;
         ptr->TickTime=pTick->TickTime;
         ptr->TickType=pTick->TickType;
         ptr->TradeBSFlag=pTick->TradeBSFlag;
         ptr->TradeMoney=pTick->TradeMoney;
         ptr->Volume=pTick->Volume;
         NGTSTick.enqueue(ptr);

    }
	void Lev2MdSpi::OnRtnTransaction(CTORATstpLev2TransactionField* pTransaction){
       CTORATstpLev2TransactionField*ptr=(CTORATstpLev2TransactionField*)je_malloc(sizeof(CTORATstpLev2TransactionField));
        if(ptr==nullptr){
        cout<<"malloc err"<<endl;
        exit(0);
        }
         ptr->BizIndex=pTransaction->BizIndex;
         ptr->BuyNo=pTransaction->BuyNo;
         ptr->ExchangeID=pTransaction->ExchangeID;
        ptr->ExecType=pTransaction->ExecType;
          ptr->Info1=pTransaction->Info1;
          ptr->Info2=pTransaction->Info2;
          ptr->Info3=pTransaction->Info3;
          ptr->MainSeq=pTransaction->MainSeq;
         strcpy( ptr->SecurityID,pTransaction->SecurityID);
          ptr->SellNo=pTransaction->SellNo;
          ptr->SubSeq=pTransaction->SubSeq;
          ptr->TradeBSFlag=pTransaction->TradeBSFlag;
          ptr->TradePrice=pTransaction->TradePrice;
          ptr->TradeTime=pTransaction->TradeTime;
          ptr->TradeVolume=pTransaction->TradeVolume;
        Transaction.enqueue(ptr);  

    }
	void Lev2MdSpi::OnRtnOrderDetail(CTORATstpLev2OrderDetailField* pOrderDetail){
        CTORATstpLev2OrderDetailField*ptr=(CTORATstpLev2OrderDetailField*)je_malloc(sizeof(CTORATstpLev2OrderDetailField));
        
        ptr->BizIndex=pOrderDetail->BizIndex;
        ptr->ExchangeID=pOrderDetail->ExchangeID;
        ptr->Info1=pOrderDetail->Info1;
        ptr->Info2=pOrderDetail->Info2;
        ptr->Info3=pOrderDetail->Info3;
        ptr->MainSeq=pOrderDetail->MainSeq;
        ptr->OrderNO=pOrderDetail->OrderNO;
        ptr->OrderStatus=pOrderDetail->OrderStatus;
        ptr->OrderTime=pOrderDetail->OrderTime;
        ptr->OrderType=pOrderDetail->OrderType;
        ptr->Price=pOrderDetail->Price;
        strcpy(ptr->SecurityID,pOrderDetail->SecurityID);
        ptr->Side=pOrderDetail->Side;
        ptr->SubSeq=pOrderDetail->SubSeq;
        ptr->Volume=pOrderDetail->Volume;
        OrderDetail.enqueue(ptr);

    }
    void Lev2MdSpi::manage_CH(){
        this->CH.clickhouse_init();//数据库初始化
        while(1){
            manage_MarketDate();
            manage_NGTSTick();
            manage_Transaction();
            manage_OrderDetail();
        }
    }
    void Lev2MdSpi::manage_MarketDate(){
       CTORATstpLev2MarketDataField*ptr=nullptr;
        if(MarketData.try_dequeue(ptr)){
                CH.insertMarketData(ptr);
                //SV.sendMarketData(ptr);
               je_free(ptr);
        }

                //cout<<"MarkData first-last="<<MarketData.first-MarketData.last-1<<endl;
    }
	void Lev2MdSpi::manage_NGTSTick(){
        CTORATstpLev2NGTSTickField*ptr=nullptr;
         if(NGTSTick.try_dequeue(ptr)){
            CH.insertNGTSTick(ptr);
            //SV.sendNGTSTick(ptr);
           je_free(ptr);
         }
    }
	void Lev2MdSpi::manage_Transaction(){
        CTORATstpLev2TransactionField*ptr=nullptr;
        if(Transaction.try_dequeue(ptr)){
            CH.insertTransaction(ptr);
            //SV.sendTransaction(ptr);
            je_free(ptr);
        }
    }
	void Lev2MdSpi::manage_OrderDetail(){
        CTORATstpLev2OrderDetailField*ptr=nullptr;
        if(OrderDetail.try_dequeue(ptr)){
            CH.insertOrderDetail(ptr);
            SV.sendOrderDetail(ptr);
            je_free(ptr);
        }
    }

