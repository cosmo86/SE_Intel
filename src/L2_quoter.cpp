#include "../include/L2_quoter.hpp"//头文件信息
Client client(ClientOptions().SetHost("localhost"));//初始化
int socked_test=0;
    //gettime
    std::string Lev2MdSpi::gettime() {
        std::time_t currentTime = std::time(nullptr);
        // 转换为本地时间
        std::tm* localTime = std::localtime(&currentTime);

        // 使用ostringstream来构建格式化的字符串
        std::ostringstream oss;
        oss << localTime->tm_year + 1900  // 年份
            << std::setfill('0') << std::setw(2) << localTime->tm_mon + 1  // 月份
            << std::setfill('0') << std::setw(2) << localTime->tm_mday; // 日期

        std::string tm_date = oss.str(); // 从ostringstream获取字符串
        return tm_date;
    }
    
	//初始化，账号，密码，连接地址
    void Lev2MdSpi::init(char * userid,char * password,char * address){
        strcpy(this->userid,userid);
        strcpy(this->password,password);
        strcpy(this->address,address);
        //m_api = CTORATstpLev2MdApi::CreateTstpLev2MdApi( TORA_TSTP_MST_MCAST,true);
        //m_api->RegisterMulticast(address,"172.16.2121.1","");//对于 UDP 连接，调用 RegisterMulticast 方法注册多播地址
        m_api=CTORATstpLev2MdApi::CreateTstpLev2MdApi(TORA_TSTP_MST_TCP,true);
        m_api->RegisterFront(address);//调用 RegisterFront 方法注册前置机地址
        m_api->RegisterSpi(this);//派生自回调接口类的实例
        
        m_api->Init();
        std::cout<<"Init yes"<<std::endl;
    }
    //登陆
    void Lev2MdSpi::OnFrontConnected(){
        std::cout<<"OnFrontConnected!"<<std::endl;
        CTORATstpReqUserLoginField req_user_login_field;
        memset(&req_user_login_field, 0, sizeof(req_user_login_field));//清空
        strcpy(req_user_login_field.LogInAccount, userid);//username
        req_user_login_field.LogInAccountType =  TORALEV2API::TORA_TSTP_LACT_UserID;//login_type
        //req_user_login_field.AuthMode = TORA_TSTP_SP_AM_Password;
        strcpy(req_user_login_field.Password, password);//password
        strcpy(req_user_login_field.UserProductInfo, "HX5ZJ0C1PV");//用户端产品信息
        std::cout<<req_user_login_field.Password<<std::endl;
        std::cout<<req_user_login_field.LogInAccount<<std::endl;
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
		//for(int i=0;i<3;i++){
		//	security_list[i]=(char *)malloc(sizeof(char)*16);
		//}
        char nonconst_id[31];
        std::cout<<"plase write number"<<std::endl;
        std::string s;
        std::cin>>s;
        strcpy(nonconst_id ,s.c_str());
        security_list[0] = nonconst_id;
		//strcpy(security_list[0],"002387");
		//strcpy(security_list[1],"600124");
		//strcpy(security_list[2],"688345");
        //Subscribe NGTS (orderdetial and trasaction together)
        int ret_nt = m_api->SubscribeNGTSTick(security_list, sizeof(security_list) / sizeof(char*), TORA_TSTP_EXD_SZSE);
        if (ret_nt == 0)
        {
            
            std::cout<<"yes 0"<<std::endl;
           // gettime();
        }
        else
        {
            std::cout<<"no"<<std::endl;
        }
		
		
		//Subscribe old orderdetial 
		int ret_od = m_api->SubscribeOrderDetail(security_list, sizeof(security_list) / sizeof(char*),TORA_TSTP_EXD_SZSE);
		if (ret_od == 0)
		{
			std::cout<<"yes 1"<<std::endl;
            
		}
		else
		{
			std::cout<<"no"<<std::endl;
		}

		//Subscribe old trasaction 
		int ret_t = m_api->SubscribeTransaction(security_list, sizeof(security_list) / sizeof(char*), TORA_TSTP_EXD_SZSE);
        if (ret_t == 0)
		{
			std::cout<<"yes 2"<<std::endl;
		}
		else
		{
			std::cout<<"no"<<std::endl;
		}

		//Subscribe to market data
		/*int ret_md = m_api->SubscribeMarketData(security_list, sizeof(security_list) / sizeof(char*), TORA_TSTP_EXD_SZSE);
		if (ret_md == 0)
		{
			std::cout<<"yes 3"<<std::endl;
		}
		else
		{
			std::cout<<"no"<<std::endl;
		}*/
    }
    
    //------------------------------------------------------------------------响应报文----------------------------------------------------------------------------------//
    void Lev2MdSpi::OnRspSubNGTSTick(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast){
        if (pRspInfo->ErrorID == 0)
        {
            printf("OnRspsubRtnNGTSTick sub market data success! \n");
            std::string sql="CREATE TABLE IF NOT EXISTS NGTSTickField_";
            sql+=gettime();
            sql+=std::string(" (ExchangeID String,SecurityID String,MainSeq Int32,SubSeq Int64,TickTime Int32,TickType String,BuyNo Int64,SellNo Int64,Price Float64,Volume Int64,TradeMoney Float64,Side String,TradeBSFlag String,MDSecurityStat String,Info1 Int32,Info2 Int32,Info3 Int32) ENGINE = MergeTree()ORDER BY (TickTime, ExchangeID, SecurityID)");            std::cout<<sql<<std::endl;
            client.Execute(sql.c_str());//建表
        }
        else
        {
         printf("sub market data fail, error_id[%d] error_msg[%s] \n", 
        pRspInfo->ErrorID, pRspInfo->ErrorMsg); }
    }
	/*void Lev2MdSpi::OnRspSubMarketData(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast){
        if (pRspInfo->ErrorID == 0)
        {
            printf("OnRspSubMarketData sub market data success! \n");
        }
        else
        {
         printf("sub market data fail, error_id[%d] error_msg[%s] \n", 
        pRspInfo->ErrorID, pRspInfo->ErrorMsg); }
    }*/
	void Lev2MdSpi::OnRspSubTransaction(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast){
        if (pRspInfo->ErrorID == 0)
        {
            printf("OnRspSubTransaction sub market data success! \n");
            std::string sql="CREATE TABLE IF NOT EXISTS TransactionField_";
            sql+=gettime();
            sql+=std::string("(ExchangeID String,SecurityID String,TradeTime Int32,TradePrice Float64,TradeVolume Int64,ExecType String,MainSeq Int32,SubSeq Int64,BuyNo Int64,SellNo Int64,Info1 Int32,Info2 Int32,Info3 Int32,TradeBSFlag String,BizIndex Int64) ENGINE = MergeTree() ORDER BY (TradeTime, MainSeq, SubSeq)");            std::cout<<sql<<std::endl;
            client.Execute(sql.c_str());//建表
        }
        else
        {
         printf("sub market data fail, error_id[%d] error_msg[%s] \n", 
        pRspInfo->ErrorID, pRspInfo->ErrorMsg); }
    }
	void Lev2MdSpi::OnRspSubOrderDetail(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast){
        if (pRspInfo->ErrorID == 0)
        {
            printf("OnRspSubOrderDetail sub market data success! \n");
            std::string sql="CREATE TABLE IF NOT EXISTS OrderDetail_";
            sql+=gettime();
            sql+=std::string("(ExchangeID String,SecurityID String,OrderTime Int32,Price Float64,Volume Int64,Side String,OrderType String,MainSeq Int32,SubSeq Int32,Info1 Int32,Info2 Int32,Info3 Int32,OrderNO Int64,OrderStatus String,BizIndex Int64) ENGINE = MergeTree() ORDER BY (OrderTime, ExchangeID, SecurityID);");            std::cout<<sql<<std::endl;
            client.Execute(sql.c_str());//建表
        }
        else
        {
         printf("sub market data fail, error_id[%d] error_msg[%s] \n", 
        pRspInfo->ErrorID, pRspInfo->ErrorMsg); }
    }
    //------------------------------------------------------------------------应答报文----------------------------------------------------------------------------------//
    /*void Lev2MdSpi::OnRtnMarketData(CTORATstpLev2MarketDataField* pDepthMarketData, const int FirstLevelBuyNum, const int FirstLevelBuyOrderVolumes[], const int FirstLevelSellNum, const int FirstLevelSellOrderVolumes[]){
       
        printf("OnRtnMarketData:security_id[%s] last_price[%f] time_stamp[%d] total_value_trade[%lld]\n",
            pDepthMarketData ->SecurityID,
            pDepthMarketData ->LastPrice,
            pDepthMarketData ->DataTimeStamp,
            pDepthMarketData ->TotalValueTrade);
	}*/
    void Lev2MdSpi::OnRtnNGTSTick(CTORATstpLev2NGTSTickField* pTick){
        
        
       //交易所代码
		std::cout<<pTick->ExchangeID<<' ';
		///证券代码
		std::cout<<pTick->SecurityID<<' ';
        ///时间
        std::cout<<pTick->TickTime<<' ';  
        ///价格
		std::cout<<pTick->Price<<' ';
		///数量
		std::cout<<pTick->Volume<<' ';
        //方向
        std::cout<<pTick->Side<<std::endl;

            // 构建插入数据的 SQL 语句
            std::string sql_insert = "INSERT INTO NGTSTickField_";
            sql_insert += gettime(); // Assuming gettime() returns the current time or some unique identifier
            sql_insert += " (ExchangeID, SecurityID, MainSeq, SubSeq, TickTime, TickType, BuyNo, SellNo, Price, Volume, TradeMoney, Side, TradeBSFlag, MDSecurityStat, Info1, Info2, Info3) VALUES (";
            sql_insert += "'" ;
            sql_insert += pTick->ExchangeID + "',";
            sql_insert += "'" ;
            sql_insert += pTick->SecurityID ;
            sql_insert += "',";
            sql_insert += "'" + std::to_string(pTick->MainSeq) + "',"; // 转换为字符串
            sql_insert += "'" + std::to_string(pTick->SubSeq) + "',"; // 转换为字符串
            sql_insert += std::to_string(pTick->TickTime) + ",";
            sql_insert += "'" ;
            sql_insert += pTick->TickType + "',";
            sql_insert += "'" + std::to_string(pTick->BuyNo) + "',"; // 转换为字符串
            sql_insert += "'" + std::to_string(pTick->SellNo) + "',"; // 转换为字符串
            sql_insert += std::to_string(pTick->Price) + ",";
            sql_insert += std::to_string(pTick->Volume) + ",";
            sql_insert += std::to_string(pTick->TradeMoney) + ",";
            sql_insert += "'" ;
            sql_insert += pTick->Side + "',";
            sql_insert += "'" ;
            sql_insert += pTick->TradeBSFlag + "',";
            sql_insert += "'" ;
            sql_insert += pTick->MDSecurityStat + "',";
            sql_insert += std::to_string(pTick->Info1) + ",";
            sql_insert += std::to_string(pTick->Info2) + ",";
            sql_insert += std::to_string(pTick->Info3) + ")";
            std::cout<<sql_insert<<std::endl;
            client.Execute(sql_insert.c_str()); // 执行插入数据的 SQL
        
    }
	void Lev2MdSpi::OnRtnTransaction(CTORATstpLev2TransactionField* pTransaction){
       // std::cout<<"OnRtnTransaction"<<std::endl;
       
             std::string sql = "INSERT INTO TransactionField_" + gettime() + " (ExchangeID, SecurityID, TradeTime, TradePrice, TradeVolume, ExecType, MainSeq, SubSeq, BuyNo, SellNo, Info1, Info2, Info3, TradeBSFlag, BizIndex) VALUES ";
            sql += "('";
            sql +=  pTransaction->ExchangeID ;
            sql+="', '" ;
            sql+= pTransaction->SecurityID;
            sql+= "', "
                + std::to_string( pTransaction->TradeTime) + ", " + std::to_string( pTransaction->TradePrice) + ", "
                + std::to_string( pTransaction->TradeVolume) + ", '" +  pTransaction->ExecType + "', "
                + std::to_string( pTransaction->MainSeq) + ", " + std::to_string( pTransaction->SubSeq) + ", "
                + std::to_string( pTransaction->BuyNo) + ", " + std::to_string( pTransaction->SellNo) + ", "
                + std::to_string( pTransaction->Info1) + ", " + std::to_string( pTransaction->Info2) + ", "
                + std::to_string( pTransaction->Info3) + ",'" +  pTransaction->TradeBSFlag + "', "
                + std::to_string( pTransaction->BizIndex) + ")";
            std::cout<<sql<<std::endl;
            client.Execute(sql.c_str()); // 执行插入数据的 SQL
             send(socked_test,sql.c_str(),sizeof(sql.c_str()),0);
    }
	void Lev2MdSpi::OnRtnOrderDetail(CTORATstpLev2OrderDetailField* pOrderDetail){
        std::cout<<"OnRtnOrderDetail"<<std::endl;
       std::string sql = "INSERT INTO OrderDetail_" + gettime() + " (ExchangeID,SecurityID,OrderTime,Price,Volume,Side,OrderType,MainSeq,SubSeq,Info1,Info2,Info3,OrderNO,OrderStatus,BizIndex) VALUES ";
            sql += "('";
            sql += pOrderDetail->ExchangeID ;
            sql+="', '" ;
            sql+= pOrderDetail->SecurityID;
            sql+= "', "
                + std::to_string( pOrderDetail->OrderTime) + ", " + std::to_string( pOrderDetail->Price) + ", "
                + std::to_string( pOrderDetail->Volume) + ", '" +  pOrderDetail->Side + "', '"
                + pOrderDetail->OrderType + "', " + std::to_string( pOrderDetail->MainSeq) + ", "
                + std::to_string( pOrderDetail->SubSeq) + ", " + std::to_string( pOrderDetail->Info1) + ", "
                + std::to_string( pOrderDetail->Info2) + ", " + std::to_string( pOrderDetail->Info3) + ", "
                + std::to_string( pOrderDetail->OrderNO) + ",'" +  pOrderDetail->OrderStatus + "', "
                + std::to_string( pOrderDetail->BizIndex) + ")";
            std::cout<<sql<<std::endl;
            client.Execute(sql.c_str()); // 执行插入数据的 SQL
            send(socked_test,sql.c_str(),sizeof(sql.c_str()),0);
    }
    int socket_init(){
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1){
        printf("socket err\n");
        return -1;
    }
    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(6000);
    saddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    int res=bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(res==-1){
        printf("bind err\n");
        return -1;
    }
    res=listen(sockfd,5);
    if(res==-1){
        printf("listen err\n");
        return -1;
    }
    return sockfd;
}
void accept_c(int c,short ev,void *arg){
    if(ev&EV_READ){
    char buff[128]={0};
    int n=recv(c,buff,127,0);
    if(n<=0){
        close(c);
        //event_free();
		return;
    }
    else{
        printf("%d is buff=%s\n",c,buff);
        send(c,"ok",2,0);
    }}
}
void accept_cb(int sockfd,short ev,void *arg){
    if(ev&EV_READ){//如果读事件
		int c=accept(sockfd,NULL,NULL);
        if(c<0){
            return;
        }
        printf("c=%d",c);
        socked_test=c;
        struct event *c_cv=event_new((struct event_base*)arg,c,EV_READ|EV_PERSIST,accept_c,NULL);
        event_add(c_cv,NULL);
    }
}

    int main(){
        Lev2MdSpi spi;
        
        char userid[21],passwd[32],addrs[64];
        strcpy(userid,"00032129");
        strcpy(passwd,"19359120");
        strcpy(addrs,"tcp://210.14.72.17:6900");
       //strcpy(addrs, "tcp://127.0.0.1:8500");
        spi.init(userid,passwd,addrs);
        sleep(3);
        spi.add();
        int sockfd=socket_init();
        if(sockfd==-1){
            exit(1);
        }
        struct event_base *base=event_init();
        if(base==NULL){
            exit(1);
        }
        struct event*sock_ev=event_new(base,sockfd,EV_READ|EV_PERSIST,accept_cb,base);
        event_add(sock_ev,NULL);
        event_base_dispatch(base);
        event_free(sock_ev);
        event_base_free(base);
        while(1){

        }
        std::cout<<"this ok"<<std::endl;
    }