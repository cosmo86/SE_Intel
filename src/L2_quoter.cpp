#include "../include/L2_quoter.hpp"//头文件信息
	//初始化，账号，密码，连接地址,数据库，网络
    void Lev2MdSpi::init(char * userid,char * password,char * address){
        strcpy(this->userid,userid);//账号
        strcpy(this->password,password);//密码
        strcpy(this->address,address);//地址
        m_api=CTORATstpLev2MdApi::CreateTstpLev2MdApi(TORA_TSTP_MST_TCP,true);
        m_api->RegisterFront(address);//调用 RegisterFront 方法注册前置机地址
        m_api->RegisterSpi(this);//派生自回调接口类的实例
        m_api->Init();
        std::cout<<"Init yes"<<std::endl;
        MarketData.Init(5000);
        NGTSTick.Init(5000);
	    Transaction.Init(5000);
	    OrderDetail.Init(5000);
    }
    //初始化网络和数据库，并接受数据库数据
    void Lev2MdSpi::init_CH_SV(){
        this->CH.clickhouse_init();//数据库初始化
        this->SV.service_init();//网络初始化
    }
    //登陆
    void Lev2MdSpi::OnFrontConnected(){
        std::cout<<"OnFrontConnected!"<<std::endl;
        CTORATstpReqUserLoginField req_user_login_field;
        memset(&req_user_login_field, 0, sizeof(req_user_login_field));//清空
        strcpy(req_user_login_field.LogInAccount, userid);//userid
        req_user_login_field.LogInAccountType =  TORALEV2API::TORA_TSTP_LACT_UserID;//login_type
        strcpy(req_user_login_field.Password, password);//password
        strcpy(req_user_login_field.UserProductInfo, "HX5ZJ0C1PV");//用户端产品信息
        int ret = m_api->ReqUserLogin(&req_user_login_field, ++m_request_id);//调用登陆函数，参数为登陆请求信息
        if (ret != 0)//登陆失败返回0
        {
            printf("ReqUserLogin fail, ret[%d]\n", ret);
        }
    }
    //登陆响应
    void Lev2MdSpi:: OnRspUserLogin(CTORATstpRspUserLoginField *pRspUserLogin, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
        if (pRspInfo->ErrorID == 0){
            printf("login success\n");
        }
        else{
            printf("login fail, error_id[%d] error_msg[%s]\n",pRspInfo ->ErrorID, pRspInfo ->ErrorMsg);
        }
    }

    void Lev2MdSpi::add(){
        char* security_list[1];
        char nonconst_id[31];
        std::cout<<"plase write number"<<std::endl;
        std::string s;
        std::cin>>s;
        strcpy(nonconst_id ,s.c_str());
        security_list[0] = nonconst_id;
        if (m_api->SubscribeNGTSTick(security_list, sizeof(security_list) / sizeof(char*), TORA_TSTP_EXD_SZSE) == 0){
            std::cout<<"add SubscribeNGTSTick success!"<<std::endl;
        }
        else{
            std::cout<<"add SubscribeNGTSTick error!"<<std::endl;
        }
		//Subscribe old orderdetial 
		if (m_api->SubscribeOrderDetail(security_list, sizeof(security_list) / sizeof(char*),TORA_TSTP_EXD_SZSE) == 0){
			std::cout<<"add SubscribeOrderDetail success!"<<std::endl;
        }
		else{
			std::cout<<"add SubscribeOrderDetail error!"<<std::endl;
		}
		//Subscribe old trasaction 
        if (m_api->SubscribeTransaction(security_list, sizeof(security_list) / sizeof(char*), TORA_TSTP_EXD_SZSE) == 0){
			std::cout<<"add SubscribeTransaction success!"<<std::endl;
		}
		else{
			std::cout<<"add SubscribeTransaction error!"<<std::endl;
		}
        if(m_api->SubscribeMarketData(security_list,sizeof(security_list) / sizeof(char*),TORA_TSTP_EXD_SZSE) ==0){
            std::cout<<"add SubscribeMarketData success!"<<std::endl;
        }
        else{
            std::cout<<"add SubscribeMarketData error!"<<std::endl;
        }
        
        
    }
    
    //------------------------------------------------------------------------响应报文----------------------------------------------------------------------------------//
    void Lev2MdSpi::OnRspSubNGTSTick(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast){
        if (pRspInfo->ErrorID == 0){
            std::cout<<"yes0"<<std::endl;
            CH.buildNGTSTick();
        }
        else{
            printf("sub market data fail, error_id[%d] error_msg[%s] \n",pRspInfo->ErrorID, pRspInfo->ErrorMsg);
        }
    }
	void Lev2MdSpi::OnRspSubTransaction(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast){
        if (pRspInfo->ErrorID == 0){
            std::cout<<"yes1"<<std::endl;
            CH.buildTransaction();
        }
        else{
         printf("sub market data fail, error_id[%d] error_msg[%s] \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
        }
    }
	void Lev2MdSpi::OnRspSubOrderDetail(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast){
        
        if (pRspInfo->ErrorID == 0){std::cout<<"yes2"<<std::endl;
           CH.buildOrderDetail();
        }
        else{
         printf("sub market data fail, error_id[%d] error_msg[%s] \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
        }
    }
    void Lev2MdSpi::OnRspSubMarketData (CTORATstpSpecificSecurityField*pSpecificSecurity, CTORATstpRspInfoField*pRspInfo, int nRequestID, bool bIsLast) {
        if (pRspInfo->ErrorID == 0){
            std::cout<<"yes3"<<std::endl;
           CH.buildMarketData();
        }
        else{
         printf("sub market data fail, error_id[%d] error_msg[%s] \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
        }
    }

    void  Lev2MdSpi::OnRtnMarketData(CTORATstpLev2MarketDataField* pDepthMarketData, const int FirstLevelBuyNum, const int FirstLevelBuyOrderVolumes[], const int FirstLevelSellNum, const int FirstLevelSellOrderVolumes[]){
        //auto start = std::chrono::high_resolution_clock::now();
        MarketData[MarketData.first].Ask10NumOrders=pDepthMarketData->Ask10NumOrders;
        MarketData[MarketData.first].Ask1NumOrders=pDepthMarketData->Ask1NumOrders;
        MarketData[MarketData.first].Ask2NumOrders=pDepthMarketData->Ask2NumOrders;
        MarketData[MarketData.first].Ask3NumOrders=pDepthMarketData->Ask3NumOrders;
        MarketData[MarketData.first].Ask4NumOrders=pDepthMarketData->Ask4NumOrders;
        MarketData[MarketData.first].Ask5NumOrders=pDepthMarketData->Ask5NumOrders;
        MarketData[MarketData.first].Ask6NumOrders=pDepthMarketData->Ask6NumOrders;
        MarketData[MarketData.first].Ask7NumOrders=pDepthMarketData->Ask7NumOrders;
        MarketData[MarketData.first].Ask8NumOrders=pDepthMarketData->Ask8NumOrders;
        MarketData[MarketData.first].Ask9NumOrders=pDepthMarketData->Ask9NumOrders;
        MarketData[MarketData.first].AskPrice10=pDepthMarketData->AskPrice10;
        MarketData[MarketData.first].AskPrice1=pDepthMarketData->AskPrice1;
        MarketData[MarketData.first].AskPrice2=pDepthMarketData->AskPrice2;
        MarketData[MarketData.first].AskPrice3=pDepthMarketData->AskPrice3;
        MarketData[MarketData.first].AskPrice4=pDepthMarketData->AskPrice4;
        MarketData[MarketData.first].AskPrice5=pDepthMarketData->AskPrice5;
        MarketData[MarketData.first].AskPrice6=pDepthMarketData->AskPrice6;
        MarketData[MarketData.first].AskPrice7=pDepthMarketData->AskPrice7;
        MarketData[MarketData.first].AskPrice8=pDepthMarketData->AskPrice8;
        MarketData[MarketData.first].AskPrice9=pDepthMarketData->AskPrice9;
        MarketData[MarketData.first].AskVolume10=pDepthMarketData->AskVolume10;
        MarketData[MarketData.first].AskVolume1=pDepthMarketData->AskVolume1;
        MarketData[MarketData.first].AskVolume2=pDepthMarketData->AskVolume2;
        MarketData[MarketData.first].AskVolume3=pDepthMarketData->AskVolume3;
        MarketData[MarketData.first].AskVolume4=pDepthMarketData->AskVolume4;
        MarketData[MarketData.first].AskVolume5=pDepthMarketData->AskVolume5;
        MarketData[MarketData.first].AskVolume6=pDepthMarketData->AskVolume6;
        MarketData[MarketData.first].AskVolume7=pDepthMarketData->AskVolume7;
        MarketData[MarketData.first].AskVolume8=pDepthMarketData->AskVolume8;
        MarketData[MarketData.first].AskVolume9=pDepthMarketData->AskVolume9;
        MarketData[MarketData.first].AvgAskPrice=pDepthMarketData->AvgAskPrice;
        MarketData[MarketData.first].AvgBidPrice=pDepthMarketData->AvgBidPrice;
        MarketData[MarketData.first].Bid10NumOrders=pDepthMarketData->Bid10NumOrders;
        MarketData[MarketData.first].Bid1NumOrders=pDepthMarketData->Bid1NumOrders;
        MarketData[MarketData.first].Bid2NumOrders=pDepthMarketData->Bid2NumOrders;
        MarketData[MarketData.first].Bid3NumOrders=pDepthMarketData->Bid3NumOrders;
        MarketData[MarketData.first].Bid4NumOrders=pDepthMarketData->Bid4NumOrders;
        MarketData[MarketData.first].Bid5NumOrders=pDepthMarketData->Bid5NumOrders;
        MarketData[MarketData.first].Bid6NumOrders=pDepthMarketData->Bid6NumOrders;
        MarketData[MarketData.first].Bid7NumOrders=pDepthMarketData->Bid7NumOrders;
        MarketData[MarketData.first].Bid8NumOrders=pDepthMarketData->Bid8NumOrders;
        MarketData[MarketData.first].Bid9NumOrders=pDepthMarketData->Bid9NumOrders;
        MarketData[MarketData.first].BidPrice10=pDepthMarketData->BidPrice10;
        MarketData[MarketData.first].BidPrice1=pDepthMarketData->BidPrice1;
        MarketData[MarketData.first].BidPrice2=pDepthMarketData->BidPrice2;
        MarketData[MarketData.first].BidPrice3=pDepthMarketData->BidPrice3;
        MarketData[MarketData.first].BidPrice4=pDepthMarketData->BidPrice4;
        MarketData[MarketData.first].BidPrice5=pDepthMarketData->BidPrice5;
        MarketData[MarketData.first].BidPrice6=pDepthMarketData->BidPrice6;
        MarketData[MarketData.first].BidPrice7=pDepthMarketData->BidPrice7;
        MarketData[MarketData.first].BidPrice8=pDepthMarketData->BidPrice8;
        MarketData[MarketData.first].BidPrice9=pDepthMarketData->BidPrice9;
        MarketData[MarketData.first].BidTradeMaxDuration=pDepthMarketData->BidTradeMaxDuration;
        MarketData[MarketData.first].BidVolume10=pDepthMarketData->BidVolume10;
        MarketData[MarketData.first].BidVolume9=pDepthMarketData->BidVolume9;
        MarketData[MarketData.first].BidVolume8=pDepthMarketData->BidVolume8;
        MarketData[MarketData.first].BidVolume7=pDepthMarketData->BidVolume7;
        MarketData[MarketData.first].BidVolume6=pDepthMarketData->BidVolume6;
        MarketData[MarketData.first].BidVolume5=pDepthMarketData->BidVolume5;
        MarketData[MarketData.first].BidVolume4=pDepthMarketData->BidVolume4;
        MarketData[MarketData.first].BidVolume3=pDepthMarketData->BidVolume3;
        MarketData[MarketData.first].BidVolume2=pDepthMarketData->BidVolume2;
        MarketData[MarketData.first].BidVolume1=pDepthMarketData->BidVolume1;
        MarketData[MarketData.first].ClosePrice=pDepthMarketData->ClosePrice;
        MarketData[MarketData.first].DataTimeStamp=pDepthMarketData->DataTimeStamp;
        MarketData[MarketData.first].ExchangeID=pDepthMarketData->ExchangeID;
        MarketData[MarketData.first].HighestPrice=pDepthMarketData->HighestPrice;
        MarketData[MarketData.first].Info1=pDepthMarketData->Info1;
        MarketData[MarketData.first].Info2=pDepthMarketData->Info2;
        MarketData[MarketData.first].Info3=pDepthMarketData->Info3;
        MarketData[MarketData.first].IOPV=pDepthMarketData->IOPV;
        MarketData[MarketData.first].LastPrice=pDepthMarketData->LastPrice;
        MarketData[MarketData.first].LowerLimitPrice=pDepthMarketData->LowerLimitPrice;
        MarketData[MarketData.first].LowestPrice=pDepthMarketData->LowestPrice;
        MarketData[MarketData.first].MDSecurityStat=pDepthMarketData->MDSecurityStat;
        MarketData[MarketData.first].NumTrades=pDepthMarketData->NumTrades;
        MarketData[MarketData.first].OfferTradeMaxDuration=pDepthMarketData->OfferTradeMaxDuration;
        MarketData[MarketData.first].OpenPrice=pDepthMarketData->OpenPrice;
        MarketData[MarketData.first].PreClosePrice=pDepthMarketData->PreClosePrice;
        strcpy(MarketData[MarketData.first].SecurityID,pDepthMarketData->SecurityID);
        MarketData[MarketData.first].UpperLimitPrice=pDepthMarketData->UpperLimitPrice;
        MarketData[MarketData.first].TotalAskVolume=pDepthMarketData->TotalAskVolume;
        MarketData[MarketData.first].TotalBidNumber=pDepthMarketData->TotalBidNumber;
        MarketData[MarketData.first].TotalBidVolume=pDepthMarketData->TotalBidVolume;
        MarketData[MarketData.first].TotalOfferNumber=pDepthMarketData->TotalOfferNumber;
        MarketData[MarketData.first].TotalValueTrade=pDepthMarketData->TotalValueTrade;
        MarketData[MarketData.first].TotalVolumeTrade=pDepthMarketData->TotalVolumeTrade;
        MarketData[MarketData.first].WithdrawBuyAmount=pDepthMarketData->WithdrawBuyAmount;
        MarketData[MarketData.first].WithdrawBuyMoney=pDepthMarketData->WithdrawBuyMoney;
        MarketData[MarketData.first].WithdrawBuyNumber=pDepthMarketData->WithdrawBuyNumber;
        MarketData[MarketData.first].WithdrawSellAmount=pDepthMarketData->WithdrawSellAmount;
        MarketData[MarketData.first].WithdrawSellMoney=pDepthMarketData->WithdrawSellMoney;
        MarketData[MarketData.first].WithdrawSellNumber=pDepthMarketData->WithdrawSellNumber;
        MarketData.first=(MarketData.first+1)%MarketData.length();
        /*auto end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::milli> duration0 = end - start;
        std::cout << "MarketData m-s took " << duration0.count() << " milliseconds." << std::endl;*/
        /*CH.insertMarketData(pDepthMarketData);
        SV.sendMarketData(pDepthMarketData);*/
    }
    void Lev2MdSpi::OnRtnNGTSTick(CTORATstpLev2NGTSTickField* pTick){
         NGTSTick[NGTSTick.first].BuyNo=pTick->BuyNo;
         NGTSTick[NGTSTick.first].ExchangeID=pTick->ExchangeID;
         NGTSTick[NGTSTick.first].Info1=pTick->Info1;
         NGTSTick[NGTSTick.first].Info2=pTick->Info2;
         NGTSTick[NGTSTick.first].Info3=pTick->Info3;
         NGTSTick[NGTSTick.first].MainSeq=pTick->MainSeq;
         NGTSTick[NGTSTick.first].MDSecurityStat=pTick->MDSecurityStat;
         NGTSTick[NGTSTick.first].Price=pTick->Price;
         strcpy(NGTSTick[NGTSTick.first].SecurityID,pTick->SecurityID);
         NGTSTick[NGTSTick.first].SellNo=pTick->SellNo;
         NGTSTick[NGTSTick.first].Side=pTick->Side;
         NGTSTick[NGTSTick.first].SubSeq=pTick->SubSeq;
         NGTSTick[NGTSTick.first].TickTime=pTick->TickTime;
         NGTSTick[NGTSTick.first].TickType=pTick->TickType;
         NGTSTick[NGTSTick.first].TradeBSFlag=pTick->TradeBSFlag;
         NGTSTick[NGTSTick.first].TradeMoney=pTick->TradeMoney;
         NGTSTick[NGTSTick.first].Volume=pTick->Volume;
         NGTSTick.first=(NGTSTick.first+1)%NGTSTick.length();
    }
	void Lev2MdSpi::OnRtnTransaction(CTORATstpLev2TransactionField* pTransaction){
         //auto start = std::chrono::high_resolution_clock::now();
         Transaction[Transaction.first].BizIndex=pTransaction->BizIndex;
         Transaction[Transaction.first].BuyNo=pTransaction->BuyNo;
         Transaction[Transaction.first].ExchangeID=pTransaction->ExchangeID;
         Transaction[Transaction.first].ExecType=pTransaction->ExecType;
         Transaction[Transaction.first].Info1=pTransaction->Info1;
         Transaction[Transaction.first].Info2=pTransaction->Info2;
         Transaction[Transaction.first].Info3=pTransaction->Info3;
         Transaction[Transaction.first].MainSeq=pTransaction->MainSeq;
         strcpy(Transaction[Transaction.first].SecurityID,pTransaction->SecurityID);
         Transaction[Transaction.first].SellNo=pTransaction->SellNo;
         Transaction[Transaction.first].SubSeq=pTransaction->SubSeq;
         Transaction[Transaction.first].TradeBSFlag=pTransaction->TradeBSFlag;
         Transaction[Transaction.first].TradePrice=pTransaction->TradePrice;
         Transaction[Transaction.first].TradeTime=pTransaction->TradeTime;
         Transaction[Transaction.first].TradeVolume=pTransaction->TradeVolume;
        Transaction.first=(Transaction.first+1)%Transaction.length();
        /* auto end = std::chrono::high_resolution_clock::now();
         std::chrono::duration<double, std::milli> duration0 = end - start;
        std::cout << "Transaction m-s took " << duration0.count() << " milliseconds." << std::endl;*/
        
    }
	void Lev2MdSpi::OnRtnOrderDetail(CTORATstpLev2OrderDetailField* pOrderDetail){
        OrderDetail[OrderDetail.first].BizIndex=pOrderDetail->BizIndex;
        OrderDetail[OrderDetail.first].ExchangeID=pOrderDetail->ExchangeID;
        OrderDetail[OrderDetail.first].Info1=pOrderDetail->Info1;
        OrderDetail[OrderDetail.first].Info2=pOrderDetail->Info2;
        OrderDetail[OrderDetail.first].Info3=pOrderDetail->Info3;
        OrderDetail[OrderDetail.first].MainSeq=pOrderDetail->MainSeq;
        OrderDetail[OrderDetail.first].OrderNO=pOrderDetail->OrderNO;
        OrderDetail[OrderDetail.first].OrderStatus=pOrderDetail->OrderStatus;
        OrderDetail[OrderDetail.first].OrderTime=pOrderDetail->OrderTime;
        OrderDetail[OrderDetail.first].OrderType=pOrderDetail->OrderType;
        OrderDetail[OrderDetail.first].Price=pOrderDetail->Price;
        strcpy(OrderDetail[OrderDetail.first].SecurityID,pOrderDetail->SecurityID);
        OrderDetail[OrderDetail.first].Side=pOrderDetail->Side;
        OrderDetail[OrderDetail.first].SubSeq=pOrderDetail->SubSeq;
        OrderDetail[OrderDetail.first].Volume=pOrderDetail->Volume;
        OrderDetail.first=(OrderDetail.first+1)%OrderDetail.length();
        /*CH.insertOrderDetail(pOrderDetail);
        SV.sendOrderDetail(pOrderDetail);*/
    }
    void Lev2MdSpi::manage_MarketDate(){
        while(1){
            if((MarketData.last+1)%MarketData.length()!=MarketData.first){
                std::unique_lock<std::mutex>lock(mtx);//锁管理器
                MarketData.last=(MarketData.last+1)%MarketData.length();
                CH.insertMarketData(&MarketData[MarketData.last]);
                SV.sendMarketData(&MarketData[MarketData.last]);
                //cout<<"MarkData first-last"<<MarketData.first-MarketData.last-1<<endl;
            }
        }
    }
	void Lev2MdSpi::manage_NGTSTick(){
      while(1){
         if((NGTSTick.last+1)%NGTSTick.length()!=NGTSTick.first){
                std::unique_lock<std::mutex>lock(mtx);//锁管理器
                NGTSTick.last=(NGTSTick.last+1)%NGTSTick.length();
                CH.insertNGTSTick(&NGTSTick[NGTSTick.last]);
                SV.sendNGTSTick(&NGTSTick[NGTSTick.last]);
                //cout<<"NGTSTick first-last="<<NGTSTick.first-NGTSTick.last-1<<endl;
            }
      }
    }
	void Lev2MdSpi::manage_Transaction(){
        while(1){
            if((Transaction.last+1)%Transaction.length()!=Transaction.first){
                std::unique_lock<std::mutex>lock(mtx);//锁管理器
                Transaction.last=(Transaction.last+1)%Transaction.length();
                CH.insertTransaction(&Transaction[Transaction.last]);
                SV.sendTransaction(&Transaction[Transaction.last]);
                //cout<<"Transaction first-last="<<Transaction.first-Transaction.last-1<<endl;
            }
        }
    }
	void Lev2MdSpi::manage_OrderDetail(){
        while(1){
            //std::unique_lock<std::mutex>lock(mtx);//锁管理器
            if((OrderDetail.last+1)%OrderDetail.length()!=OrderDetail.first){
                std::unique_lock<std::mutex>lock(mtx);//锁管理器
                OrderDetail.last=(OrderDetail.last+1)%OrderDetail.length();
                CH.insertOrderDetail(&OrderDetail[OrderDetail.last]);
                SV.sendOrderDetail(&OrderDetail[OrderDetail.last]);
               // cout<<"OrderDetail first-last="<<OrderDetail.first-OrderDetail.last-1<<endl;
            }
        }
    }

