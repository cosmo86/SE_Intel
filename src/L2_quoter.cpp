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

    void  Lev2MdSpi::OnRtnMarketData (CTORATstpLev2MarketDataField *pMarketData, const int *FirstLevelBuyNum, const int FirstLevelBuyOrderVolumes[],const int *FirstLevelSellNum, const int FirstLevelSellOrderVolumes[]){
       std::cout<<1<<std::endl;
        CH.insertMarketData(pMarketData);
        SV.sendMarketData(pMarketData);
    }
    void Lev2MdSpi::OnRtnNGTSTick(CTORATstpLev2NGTSTickField* pTick){
        std::cout<<2<<std::endl;
        CH.insertNGTSTick(pTick);
        SV.sendNGTSTick(pTick);
    }
	void Lev2MdSpi::OnRtnTransaction(CTORATstpLev2TransactionField* pTransaction){
       // std::cout<<3<<std::endl;
        CH.insertTransaction(pTransaction);
        SV.sendTransaction(pTransaction);
    }
	void Lev2MdSpi::OnRtnOrderDetail(CTORATstpLev2OrderDetailField* pOrderDetail){
         //std::cout<<4<<std::endl;
        CH.insertOrderDetail(pOrderDetail);
        SV.sendOrderDetail(pOrderDetail);
    }

