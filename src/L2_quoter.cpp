#include "../include/L2_quoter.hpp"//头文件信息
Client client(ClientOptions().SetHost("localhost"));//初始化
//int socked_test=0;
//int cishu=0;
server echo_server;
std::vector<websocketpp::connection_hdl>socked_V;

std::string timestampToString(std::time_t timestamp) {
    std::time_t currentTime = std::time(nullptr);
    std::tm* tm_info = std::localtime(&currentTime);
    std::ostringstream oss;
    oss << std::put_time(tm_info, "%Y-%m-%dT%H:%M:%S");
    return oss.str();
}
    std::string Lev2MdSpi::gettime() {
      /* std::time_t currentTime = std::time(nullptr);
        // 转换为本地时间
        std::tm* localTime = std::localtime(&currentTime);

        // 使用ostringstream来构建格式化的字符串
        std::ostringstream oss;
        oss << localTime->tm_year + 1900  // 年份
            << std::setfill('0') << std::setw(2) << localTime->tm_mon + 1  // 月份
            << std::setfill('0') << std::setw(2) << localTime->tm_mday; // 日期

        std::string tm_date = oss.str(); // 从ostringstream获取字符串
        return tm_date;*/
        return "20240416";
    }
	//初始化，账号，密码，连接地址,数据库，网络
    void Lev2MdSpi::init(char * userid,char * password,char * address){
        strcpy(this->userid,userid);//账号
        strcpy(this->password,password);//密码
        strcpy(this->address,address);//地址
        m_api=CTORATstpLev2MdApi::CreateTstpLev2MdApi(TORA_TSTP_MST_TCP,true);
        m_api->RegisterFront(address);//调用 RegisterFront 方法注册前置机地址
        m_api->RegisterSpi(this);//派生自回调接口类的实例
        m_api->Init();
        CH.clickhouse_init();//数据库初始化
        SV.service_init();//网络初始化
        std::cout<<"Init yes"<<std::endl;
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
    }
    
    //------------------------------------------------------------------------响应报文----------------------------------------------------------------------------------//
    void Lev2MdSpi::OnRspSubNGTSTick(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast){
        if (pRspInfo->ErrorID == 0){
            CH.buildNGTSTick();
        }
        else{
            printf("sub market data fail, error_id[%d] error_msg[%s] \n",pRspInfo->ErrorID, pRspInfo->ErrorMsg);
        }
    }
	void Lev2MdSpi::OnRspSubTransaction(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast){
        if (pRspInfo->ErrorID == 0){
            CH.buildTransaction();
        }
        else{
         printf("sub market data fail, error_id[%d] error_msg[%s] \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
        }
    }
	void Lev2MdSpi::OnRspSubOrderDetail(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast){
        if (pRspInfo->ErrorID == 0){
           CH.buildOrderDetail();
        }
        else{
         printf("sub market data fail, error_id[%d] error_msg[%s] \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
        }
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
        CH.insertNGTSTick(pTick);
        SV.service.send()
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
            client.Execute(sql.c_str()); // 执行插入数据的 SQL
            std::cout<<pTransaction->TradeTime<<std::endl;
            for(int i=0;i<socked_V.size();i++){
                std::thread t(send_json_packet,pTransaction ,&echo_server, socked_V[i]);
                 t.detach(); // Detach the thread to run independently
            }
    }
	void Lev2MdSpi::OnRtnOrderDetail(CTORATstpLev2OrderDetailField* pOrderDetail){
        //std::cout<<"OnRtnOrderDetail"<<std::endl;
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
            client.Execute(sql.c_str()); // 执行插入数据的 SQL
    }
    void test::print(test* p){
        std::cout<<"type=2"<<std::endl;
        std::cout<<"a="<<p->a<<std::endl;
        std::cout<<"b="<<p->b<<std::endl;
        std::cout<<"c="<<p->c<<std::endl;
    }
    void test::print_zidingyi(){
        std::cout<<"type=3"<<std::endl;
    }
void test_print(){
    test ceshi;
    ceshi.print();
}
void test_print_zidingyi(){
    test ceshi;
    ceshi.print_zidingyi();
}
void test_print_p(Json::Value &val){
    int a=val["val"]["a"].asInt();
    double b=val["val"]["b"].asDouble();
    std::string c=val["val"]["c"].asCString();
    test *p=new test(a,b,c);
    test ceshi;
    ceshi.print(p);
}

void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg) {
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;

    // Check for a special command to instruct the server to stop listening
    if (msg->get_payload() == "stop-listening") {
        s->stop_listening();
        return;
    }

    // Construct the response message
    std::string response = "Service response: " + msg->get_payload();

    try {
        // Send the response message back to the client
        s->send(hdl, response, websocketpp::frame::opcode::text);
    } catch (const websocketpp::exception& e) {
        std::cout << "Send failed because: " << e.what() << std::endl;
    }
}

// Function to send JSON packet every second
void send_json_packet(CTORATstpLev2TransactionField* pTransaction,server* s, websocketpp::connection_hdl hdl) {
    if(pTransaction->TradePrice==0)return;
    Json::Value val;
    val["ExchangeId"]=pTransaction->ExchangeID;
    val["ExchangeID"]=pTransaction->ExchangeID;
    val["SecurityID"]=pTransaction->SecurityID;
    val["TradeTime"]= timestampToString(pTransaction->TradeTime);
    //val["TradeTime"]=pTransaction->TradeTime;
    val["TradePrice"]=pTransaction->TradePrice;
    val["TradeVolume"]=std::to_string(pTransaction->TradeVolume);
    val["ExecType"]=pTransaction->ExecType;
    val["MainSeq"]=pTransaction->MainSeq;
    val["SubSeq"]=std::to_string(pTransaction->SubSeq);
    val["SellNo"]=std::to_string(pTransaction->SellNo);
    val["Info1"]=pTransaction->Info1;
    val["Info2"]=pTransaction->Info2;
    val["Info3"]=pTransaction->Info3;
    val["TradeBSFlag"]=pTransaction->TradeBSFlag;
    val["BizIndex"]=std::to_string(pTransaction->BizIndex);
    std::cout<<val["TradeTime"]<<std::endl;
    
    try {
            // Send the JSON packet to the client
            s->send(hdl, val.toStyledString().c_str(), websocketpp::frame::opcode::text);
        } catch (const websocketpp::exception& e) {
            std::cout << "Send failed because: " << e.what() << std::endl;
    }
    //ws://91.208.73.166:9002
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
        while(1){}
        std::cout<<"this ok"<<std::endl;
    }
/*int socket_init(){
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1){
        printf("socket err\n");
        return -1;
    }
    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(6000);
    saddr.sin_addr.s_addr=inet_addr("91.208.73.166");
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
}*/
/*void accept_c(int c,short ev,void *arg){
    if(ev&EV_READ){
        char buff[1024]={0};
        int n=recv(c,buff,1023,0);
        
        std::cout<<n<<">>>>>:";
        for(int i=0;i<n;i++){
            std::cout<<buff[i];
        }
        if(n<=0){
            std::cout<<"errrrrr"<<std::endl;
            close(c);
            //event_free();
            return;
        }
        else{
            std::cout<<5201314<<std::endl;
             std::string websocketHandshake = "HTTP/1.1 101 Switching Protocols\nUpgrade: websocket\nConnection: Upgrade\nSec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\nSec-WebSocket-Protocol: chat";

// 构建TLS握手头部
std::string tlsHandshakeHeader = "TLS/1.2 200 OK\nContent-Type: application/octet-stream\n";

// 计算TLS握手消息体的长度
std::string tlsHandshakeBody = websocketHandshake;
int bodyLength = tlsHandshakeBody.size();

// 构建TLS握手头部，指定消息体长度
tlsHandshakeHeader += "Content-Length: " + std::to_string(bodyLength) + "\n\n";

// 计算TLS握手消息的校验和
// 假设这里使用SHA256进行哈希计算
std::string checksum = sha256(tlsHandshakeBody); // 自定义函数，用于计算SHA256校验和

// 构建最终的TLS握手消息
    std::string tlsHandshake = tlsHandshakeHeader + tlsHandshakeBody + checksum;
    //std::string ans=std::string("HTTP/1.1 101 Switching Protocols\nUpgrade: websocket\nConnection: Upgrade\nSec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\nSec-WebSocket-Protocol: chat");
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    if (getsockname(c, (struct sockaddr*)&addr, &addr_len) < 0) {
        std::cerr << "Error getting socket name" << std::endl;
        close(c);
        return;
    }
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);

    std::cout << "Local IP address: " << ip << std::endl;
    std::cout << "Local port: " << ntohs(addr.sin_port) << std::endl;
        send(c,tlsHandshake.c_str(),strlen(tlsHandshake.c_str()),0);
        }
    }
}*/
/*void accept_cb(int sockfd,short ev,void *arg){
    if(ev&EV_READ){//如果读事件
		int c=accept(sockfd,NULL,NULL);
        if(c<0){
            return;
        }
        //printf("c=%d",c);
       
        struct event *c_cv=event_new((struct event_base*)arg,c,EV_READ|EV_PERSIST,accept_c,NULL);
        event_add(c_cv,NULL);
    }
}*/