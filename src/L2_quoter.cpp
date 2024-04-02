#include "../include/L2_quoter.hpp"//头文件信息
    
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
		int ret_md = m_api->SubscribeMarketData(security_list, sizeof(security_list) / sizeof(char*), TORA_TSTP_EXD_SZSE);
		if (ret_md == 0)
		{
			std::cout<<"yes 3"<<std::endl;
		}
		else
		{
			std::cout<<"no"<<std::endl;
		}
    }
    
    //------------------------------------------------------------------------响应报文----------------------------------------------------------------------------------//
    void Lev2MdSpi::OnRspSubNGTSTick(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast){
        if (pRspInfo->ErrorID == 0)
        {
            printf("OnRspsubRtnNGTSTick sub market data success! \n");
        }
        else
        {
         printf("sub market data fail, error_id[%d] error_msg[%s] \n", 
        pRspInfo->ErrorID, pRspInfo->ErrorMsg); }
    }
	void Lev2MdSpi::OnRspSubMarketData(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast){
        if (pRspInfo->ErrorID == 0)
        {
            printf("OnRspSubMarketData sub market data success! \n");
        }
        else
        {
         printf("sub market data fail, error_id[%d] error_msg[%s] \n", 
        pRspInfo->ErrorID, pRspInfo->ErrorMsg); }
    }
	void Lev2MdSpi::OnRspSubTransaction(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast){
        if (pRspInfo->ErrorID == 0)
        {
            printf("OnRspSubTransaction sub market data success! \n");
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
        }
        else
        {
         printf("sub market data fail, error_id[%d] error_msg[%s] \n", 
        pRspInfo->ErrorID, pRspInfo->ErrorMsg); }
    }
    //------------------------------------------------------------------------应答报文----------------------------------------------------------------------------------//
    void Lev2MdSpi::OnRtnMarketData(CTORATstpLev2MarketDataField* pDepthMarketData, const int FirstLevelBuyNum, const int FirstLevelBuyOrderVolumes[], const int FirstLevelSellNum, const int FirstLevelSellOrderVolumes[]){
       
        /*printf("OnRtnMarketData:security_id[%s] last_price[%f] time_stamp[%d] total_value_trade[%lld]\n",
            pDepthMarketData ->SecurityID,
            pDepthMarketData ->LastPrice,
            pDepthMarketData ->DataTimeStamp,
            pDepthMarketData ->TotalValueTrade);*/
	}
    void Lev2MdSpi::OnRtnNGTSTick(CTORATstpLev2NGTSTickField* pTick){
        
       /* ///交易所代码
		std::cout<<pTick->ExchangeID<<' ';
		///证券代码
		std::cout<<pTick->SecurityID<<' ';
        ///时间
        std::cout<<pTick->	TickTime<<' ';
        ///价格
		std::cout<<pTick->	Price<<' ';
		///数量
		std::cout<<pTick->Volume<<' ';
        //方向
        std::cout<<pTick->	Side<<std::endl;*/
    }
	void Lev2MdSpi::OnRtnTransaction(CTORATstpLev2TransactionField* pTransaction){
        std::cout<<"OnRtnTransaction"<<" ";
       
        		///交易所代码
		std::cout<<pTransaction->ExchangeID<<' ';

		///证券代码
		std::cout<<pTransaction->	SecurityID<<' ';

		///时间戳
		std::cout<<pTransaction->	TradeTime<<' ';

		///成交价格
	std::cout<<pTransaction->	TradePrice<<' ';

		///成交数量
		std::cout<<pTransaction->	TradeVolume<<' ';

		///成交类别（只有深圳行情有效）
		std::cout<<pTransaction->	ExecType<<std::endl;
    }
	void Lev2MdSpi::OnRtnOrderDetail(CTORATstpLev2OrderDetailField* pOrderDetail){
        std::cout<<"OnRtnOrderDetail"<<" ";
       ///交易所代码
		std::cout<<	pOrderDetail->  ExchangeID<<' ';

		///证券代码
		std::cout<<	pOrderDetail->  SecurityID<<' ';

		///时间戳
		std::cout<<pOrderDetail->  OrderTime<<' ';

		///委托价格
		std::cout<<pOrderDetail->  Price<<' ';

		///委托数量
		std::cout<<	pOrderDetail->  Volume<<' ';

		///委托方向
		std::cout<<pOrderDetail->  Side<<std::endl;
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
        Client client(ClientOptions().SetHost("localhost"));//初始化客户端连接
        client.Execute("CREATE TABLE IF NOT EXISTS default.numbers (id UInt64, name String) ENGINE = Memory");//创建表格
             /// 插入一些数值。
        {
            Block block;

            auto id = std::make_shared<ColumnUInt64>();
            id->Append(1);
            id->Append(7);

            auto name = std::make_shared<ColumnString>();
            name->Append("one");
            name->Append("two");

            block.AppendColumn("id", id);
            block.AppendColumn("name", name);

            client.Insert("default.numbers", block);
        }

        /// 查询前面插入的数值。
        client.Select("SELECT id, name FROM default.numbers", [](const Block& block)
            {
                for (size_t i = 0; i < block.GetRowCount(); ++i) {
                    std::cout << block[0]->As<ColumnUInt64>()->At(i) << " "
                            << block[1]->As<ColumnString>()->At(i) << "\n";
                }
            }
        );

        /// 删除表格。
        client.Execute("DROP TABLE default.numbers");
        while(1){

        }
        std::cout<<"this ok"<<std::endl;
    }