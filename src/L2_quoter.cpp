#include "L2_quoter.hpp"
    
	//request
    //username password Logintype
    void Lev2MdSpi::OnFrontConnected(std::string username,std::string password){
        CTORATstpReqUserLoginField req_user_login_field;
        memset(&req_user_login_field, 0, sizeof(req_user_login_field));//清空
        strcpy(req_user_login_field.LogInAccount, username.c_str());//username
        req_user_login_field.LogInAccountType =  TORA_TSTP_LACT_UserID;//login_type
        //req_user_login_field.AuthMode = TORA_TSTP_SP_AM_Password;
        strcpy(req_user_login_field.Password, password.c_str());//password
        strcpy(req_user_login_field.UserProductInfo, "lev2apidemo");//用户端产品信息
        int ret = m_api->ReqUserLogin(&req_user_login_field, ++m_request_id);//调用登陆函数，参数为登陆请求信息
        if (ret != 0)//登陆失败返回0
        {
            printf("ReqUserLogin fail, ret[%d]\n", ret);
        }
    }
     void Lev2MdSpi:: OnRspUserLogin(CTORATstpRspUserLoginField *pRspUserLogin, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
        {
        if (pRspInfo->ErrorID == 0)
        {
            printf("login success\n");
        }
        else
        {
            printf("login fail, error_id[%d] error_msg[%s]\n",pRspInfo ->ErrorID, pRspInfo ->ErrorMsg);
        }
    }
	/// Error response
    /*
	void Lev2MdSpi::OnRspError(TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	/// Login request response，登陆响应
	void Lev2MdSpi::OnRspUserLogin(TORALEV2API::CTORATstpRspUserLoginField* pRspUserLogin, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
    // Subscribe 
	void Lev2MdSpi::Subscribe( char* ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID);
    //UnSubscribe
	void Lev2MdSpi::UnSubscribe(char* ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID);
	/*********************************** Return Data Callback Function**********************************
	/// Logout
    void Lev2MdSpi::OnRspUserLogout(TORALEV2API::CTORATstpUserLogoutField* pUserLogout, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	/// Subscribe NGTSTICK
	void Lev2MdSpi::OnRspSubNGTSTick(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
    */
	/// OnRspSubMarketData
    void Lev2MdSpi::OnRspSubMarketData(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast){
        if (pRspInfo->ErrorID == 0)
        {
        printf("sub market data success! \n");
        }
        else
        {
        printf("sub market data fail, error_id[%d] error_msg[%s] \n", 
        pRspInfo->ErrorID, pRspInfo->ErrorMsg); }
}
	/// OnRspUnSubMarketData
	/*void Lev2MdSpi::OnRspUnSubMarketData(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	// OnRspSubTransaction
	void Lev2MdSpi::OnRspSubTransaction(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	/// OnRspSubOrderDetail
	void Lev2MdSpi::OnRspSubOrderDetail(CTORATstpSpecificSecurityField* pSpecificSecurity, TORALEV2API::CTORATstpRspInfoField* pRspInfo, int nRequestID, bool bIsLast);*/
	/*********************************** Data call back ***********************************/
	/// OnRtnMarketData
    //
    void Lev2MdSpi::OnRtnMarketData(CTORATstpLev2MarketDataField* pDepthMarketData, const int FirstLevelBuyNum, const int FirstLevelBuyOrderVolumes[], const int FirstLevelSellNum, const int FirstLevelSellOrderVolumes[]){
        printf("OnRtnMarketData:security_id[%s] last_price[%f] time_stamp[%d] total_value_trade[%lld]\n",
            pDepthMarketData ->SecurityID,
            pDepthMarketData ->LastPrice,
            pDepthMarketData ->DataTimeStamp,
            pDepthMarketData ->TotalValueTrade);
	}//NGT ֪ͨ
	/*void Lev2MdSpi::OnRtnNGTSTick(CTORATstpLev2NGTSTickField* pTick);
	/// OnRtnTransaction
	void Lev2MdSpi::OnRtnTransaction(CTORATstpLev2TransactionField* pTransaction);
	/// OnRtnOrderDetail
	void Lev2MdSpi::OnRtnOrderDetail(CTORATstpLev2OrderDetailField* pOrderDetail);*/
    int main(){
        CTORATstpLev2MdApi *api = CTORATstpLev2MdApi::CreateTstpLev2MdApi();
        Lev2MdSpi spi(api);
        api->RegisterSpi(&spi);
        api->RegisterFront("tcp://127.0.0.1:8500");
        api->Init();
        std::cout<<"???"<<std::endl;
    }