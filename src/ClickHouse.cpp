#include "../include//ClickHouse.hpp"
std::string ClickHouse:: getCurrentDate() {
    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);
    // 构建格式化的日期字符串
    char dateBuffer[9]; // 8个字符 + 1个终止符
    std::sprintf(dateBuffer, "%04d%02d%02d", 
        localTime->tm_year + 1900, // 年份
        localTime->tm_mon + 1,     // 月份
        localTime->tm_mday         // 日期
    );
    std::string tm_date(dateBuffer);
    return tm_date;
}
void ClickHouse::buildNGTSTick(){
    std::string sql = "CREATE TABLE IF NOT EXISTS NGTSTickField_";
    sql += getCurrentDate();
    sql += std::string(" (ExchangeID String,SecurityID String,MainSeq Int32,SubSeq Int64,TickTime Int32,TickType String,BuyNo Int64,SellNo Int64,Price Float64,Volume Int64,TradeMoney Float64,Side String,TradeBSFlag String,MDSecurityStat String,Info1 Int32,Info2 Int32,Info3 Int32) ENGINE = MergeTree()ORDER BY (TickTime, ExchangeID, SecurityID)");
    client.Execute(sql.c_str());//建表
}
void ClickHouse::buildTransaction(){
    std::string sql="CREATE TABLE IF NOT EXISTS TransactionField_";
    sql+=getCurrentDate();
    sql+=std::string("(ExchangeID String,SecurityID String,TradeTime Int32,TradePrice Float64,TradeVolume Int64,ExecType String,MainSeq Int32,SubSeq Int64,BuyNo Int64,SellNo Int64,Info1 Int32,Info2 Int32,Info3 Int32,TradeBSFlag String,BizIndex Int64) ENGINE = MergeTree() ORDER BY (TradeTime, MainSeq, SubSeq)");
    client.Execute(sql.c_str());//建表
}
void ClickHouse::buildOrderDetail(){
    std::string sql="CREATE TABLE IF NOT EXISTS OrderDetail_";
    sql+=getCurrentDate();
    sql+=std::string("(ExchangeID String,SecurityID String,OrderTime Int32,Price Float64,Volume Int64,Side String,OrderType String,MainSeq Int32,SubSeq Int32,Info1 Int32,Info2 Int32,Info3 Int32,OrderNO Int64,OrderStatus String,BizIndex Int64) ENGINE = MergeTree() ORDER BY (OrderTime, ExchangeID, SecurityID);");
    client.Execute(sql.c_str());//建表
}
void ClickHouse::buildMarketData(){
	std::string sql="CREATE TABLE IF NOT EXISTS MarketData_";
	sql+=getCurrentDate();
	sql+=std::string("(SecurityID String,ExchangeID String,DataTimeStamp Int32,PreClosePrice Float64,OpenPrice Float64,NumTrades Int64,TotalVolumeTrade Int64,TotalValueTrade Float64,TotalBidVolume Int64,AvgBidPrice Float64,TotalAskVolume Int64,AvgAskPrice Float64,HighestPrice Float64,LowestPrice Float64,LastPrice Float64,BidPrice1 Float64,BidVolume1 Int64,AskPrice1 Float64,AskVolume1 Int64,AskPrice2 Float64,AskVolume2 Int64,AskPrice3 Float64,AskVolume3 Int64,BidPrice2 Float64,BidVolume2 Int64,BidPrice3 Float64,BidVolume3 Int64,AskPrice4 Float64,AskVolume4 Int64,AskPrice5 Float64,AskVolume5 Int64,BidPrice4 Float64,BidVolume4 Int64,BidPrice5 Float64,BidVolume5 Int64,AskPrice6 Float64,AskVolume6 Int64,AskPrice7 Float64,AskVolume7 Int64,BidPrice6 Float64,BidVolume6 Int64,BidPrice7 Float64,BidVolume7 Int64,AskPrice8 Float64,AskVolume8 Int64,AskPrice9 Float64,AskVolume9 Int64,BidPrice8\
 Float64,BidVolume8 Int64,BidPrice9 Float64,BidVolume9 Int64,BidPrice10 Float64,BidVolume10 Int64,AskPrice10 Float64,AskVolume10 Int64,Info1 Int32,Info2 Int32,Info3 Int32,UpperLimitPrice Float64,LowerLimitPrice Float64,ClosePrice Float64,MDSecurityStat String,TotalBidNumber Int32,TotalOfferNumber Int32,BidTradeMaxDuration Int32,OfferTradeMaxDuration Int32,IOPV Int32,Ask1NumOrders Int32,Bid1NumOrders Int32,Ask2NumOrders Int32,Bid2NumOrders Int32,Ask3NumOrders  Int32,Bid3NumOrders Int32,Ask4NumOrders Int32,Bid4NumOrders Int32,Ask5NumOrders Int32,Bid5NumOrders Int32,Ask6NumOrders Int32,Bid6NumOrders Int32,Ask7NumOrders Int32,Bid7NumOrders Int32,Ask8NumOrders Int32,Bid8NumOrders Int32,Ask9NumOrders Int32,Bid9NumOrders Int32,Ask10NumOrders Int32,Bid10NumOrders Int32,WithdrawBuyNumber Int32,WithdrawBuyAmount Int64,WithdrawBuyMoney Float64,WithdrawSellNumber Int32,WithdrawSellAmount Int64,WithdrawSellMoney Float64)ENGINE = MergeTree() ORDER BY (DataTimeStamp)");
	client.Execute(sql.c_str()); // 建表
}
void ClickHouse::execute(std::string sql){
    client.Execute(sql.c_str());
}
void ClickHouse::ExcuteTransaction(){
	std::string sql = "INSERT INTO TransactionField_" + getCurrentDate() + " (ExchangeID, SecurityID, TradeTime, TradePrice, TradeVolume, ExecType, MainSeq, SubSeq, BuyNo, SellNo, Info1, Info2, Info3, TradeBSFlag, BizIndex) VALUES ";
	while(!transactionQueue.empty()){
		sql+=transactionQueue.front();
		transactionQueue.pop();
	}
	client.Execute(sql.c_str()); // 执行插入数据的 SQL
}
void ClickHouse::insertTransaction(CTORATstpLev2TransactionField* pTransaction){
    std::string sql="";
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
    transactionQueue.push(sql);
	if(transactionQueue.size()>200)ExcuteTransaction();
}
void ClickHouse::ExcuteOrderDetail(){
	std::string sql = "INSERT INTO OrderDetail_" + getCurrentDate() + " (ExchangeID,SecurityID,OrderTime,Price,Volume,Side,OrderType,MainSeq,SubSeq,Info1,Info2,Info3,OrderNO,OrderStatus,BizIndex) VALUES ";
	while(!orderDetailQueue.empty()){
		sql+=orderDetailQueue.front();
		orderDetailQueue.pop();
	}
	client.Execute(sql.c_str()); // 执行插入数据的 SQL
}
void ClickHouse::insertOrderDetail(CTORATstpLev2OrderDetailField* pOrderDetail){
    std::string sql="";
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
    orderDetailQueue.push(sql);
	if(orderDetailQueue.size()>200)ExcuteOrderDetail();
}
void ClickHouse::ExcuteNGTSTick(){
	std::string sql_insert = "INSERT INTO NGTSTickField_"+ getCurrentDate() + " (ExchangeID, SecurityID, MainSeq, SubSeq, TickTime, TickType, BuyNo, SellNo, Price, Volume, TradeMoney, Side, TradeBSFlag, MDSecurityStat, Info1, Info2, Info3) VALUES";
	while(!ngtstickQueue.empty()){
		sql_insert+=ngtstickQueue.front();
		ngtstickQueue.pop();
	}
	client.Execute(sql_insert.c_str()); // 执行插入数据的 SQL
}
void ClickHouse::insertNGTSTick(CTORATstpLev2NGTSTickField* pTick){
	std::string sql_insert="";
	sql_insert += getCurrentDate(); // Assuming gettime() returns the current time or some unique identifier
	sql_insert +=  "('";
	sql_insert += pTick->ExchangeID + "','";
	sql_insert += pTick->SecurityID ;
	sql_insert += "','";
	sql_insert += std::to_string(pTick->MainSeq) + "',"; // 转换为字符串
	sql_insert += "'" + std::to_string(pTick->SubSeq) + "',"; // 转换为字符串
	sql_insert += std::to_string(pTick->TickTime) + ",'";
	sql_insert += pTick->TickType + "',";
	sql_insert += "'" + std::to_string(pTick->BuyNo) + "',"; // 转换为字符串
	sql_insert += "'" + std::to_string(pTick->SellNo) + "',"; // 转换为字符串
	sql_insert += std::to_string(pTick->Price) + ",";
	sql_insert += std::to_string(pTick->Volume) + ",";
	sql_insert += std::to_string(pTick->TradeMoney) + ",'";
	sql_insert += pTick->Side + "','";
	sql_insert += pTick->TradeBSFlag + "','";
	sql_insert += pTick->MDSecurityStat + "',";
	sql_insert += std::to_string(pTick->Info1) + ",";
	sql_insert += std::to_string(pTick->Info2) + ",";
	sql_insert += std::to_string(pTick->Info3) + ")";
	ngtstickQueue.push(sql_insert);
	if(ngtstickQueue.size()>200)ExcuteNGTSTick();
}
void ClickHouse::ExcuteMarketData(){
    std::string insert_sql="INSERT INTO MarketData_";
	insert_sql+=getCurrentDate();
	insert_sql+=" (SecurityID,ExchangeID,DataTimeStamp,PreClosePrice,OpenPrice,NumTrades,TotalVolumeTrade,TotalValueTrade,TotalBidVolume,AvgBidPrice,TotalAskVolume,AvgAskPrice,HighestPrice,LowestPrice,LastPrice,BidPrice1,BidVolume1,AskPrice1,AskVolume1,AskPrice2,AskVolume2,AskPrice3,AskVolume3,BidPrice2,BidVolume2,BidPrice3,BidVolume3,AskPrice4,AskVolume4,AskPrice5,AskVolume5,BidPrice4,BidVolume4,BidPrice5,BidVolume5,AskPrice6,AskVolume6,AskPrice7,AskVolume7,BidPrice6,BidVolume6,BidPrice7,BidVolume7,AskPrice8,AskVolume8,AskPrice9,AskVolume9,BidPrice8,BidVolume8,BidPrice9,BidVolume9,BidPrice10,BidVolume10,AskPrice10,AskVolume10,Info1,Info2,Info3,UpperLimitPrice,LowerLimitPrice,ClosePrice,MDSecurityStat,TotalBidNumber,TotalOfferNumber,BidTradeMaxDuration,OfferTradeMaxDuration,IOPV,Ask1NumOrders,Bid1NumOrders,Ask2NumOrders,Bid2NumOrders,Ask3NumOrders,Bid3NumOrders,Ask4NumOrders,Bid4NumOrders,Ask5NumOrders,Bid5NumOrders,Ask6NumOrders,Bid6NumOrders,Ask7NumOrders,Bid7NumOrders,Ask8NumOrders,Bid8NumOrders,Ask9NumOrders,Bid9NumOrders,Ask10NumOrders,Bid10NumOrders,WithdrawBuyNumber,WithdrawBuyAmount,WithdrawBuyMoney,WithdrawSellNumber,WithdrawSellAmount,WithdrawSellMoney) VALUES ";
	while(!marketDataQueue.empty()){
		insert_sql+=marketDataQueue.front();
		marketDataQueue.pop();
	}
	client.Execute(insert_sql.c_str()); // 执行插入数据的 SQL
}
void ClickHouse::insertMarketData(CTORATstpLev2MarketDataField* pMarketData){
	std::string insert_sql="";
		insert_sql+="('";
		insert_sql+=pMarketData->SecurityID;
		insert_sql+="','";
		insert_sql+=pMarketData->ExchangeID;
		insert_sql+="',";
		insert_sql+=std::to_string(pMarketData->DataTimeStamp)+",";
		insert_sql+=std::to_string(pMarketData->PreClosePrice)+",";
		insert_sql+=std::to_string(pMarketData->OpenPrice)+",";
		insert_sql+=std::to_string(pMarketData->NumTrades)+",";
		insert_sql+=std::to_string(pMarketData->TotalVolumeTrade)+",";
		insert_sql+=std::to_string(pMarketData->TotalValueTrade)+",";
		insert_sql+=std::to_string(pMarketData->TotalBidVolume)+",";
		insert_sql+=std::to_string(pMarketData->AvgBidPrice)+",";
		insert_sql+=std::to_string(pMarketData->TotalAskVolume)+",";
		insert_sql+=std::to_string(pMarketData->AvgAskPrice)+",";
		insert_sql+=std::to_string(pMarketData->HighestPrice)+",";
		insert_sql+=std::to_string(pMarketData->LowestPrice)+",";
		insert_sql+=std::to_string(pMarketData->LastPrice)+",";
		insert_sql+=std::to_string(pMarketData->BidPrice1)+",";
		insert_sql+=std::to_string(pMarketData->BidVolume1)+",";
		insert_sql+=std::to_string(pMarketData->AskPrice1)+",";
		insert_sql+=std::to_string(pMarketData->AskVolume1)+",";
		insert_sql+=std::to_string(pMarketData->AskPrice2)+",";
		insert_sql+=std::to_string(pMarketData->AskVolume2)+",";
		insert_sql+=std::to_string(pMarketData->AskPrice3)+",";
		insert_sql+=std::to_string(pMarketData->AskVolume3)+",";
		insert_sql+=std::to_string(pMarketData->BidPrice2)+",";
		insert_sql+=std::to_string(pMarketData->BidVolume2)+",";
		insert_sql+=std::to_string(pMarketData->BidPrice3)+",";
		insert_sql+=std::to_string(pMarketData->BidVolume3)+",";
		insert_sql+=std::to_string(pMarketData->AskPrice4)+",";
		insert_sql+=std::to_string(pMarketData->AskVolume4)+",";
		insert_sql+=std::to_string(pMarketData->AskPrice5)+",";
		insert_sql+=std::to_string(pMarketData->AskVolume5)+",";
		insert_sql+=std::to_string(pMarketData->BidPrice4)+",";
		insert_sql+=std::to_string(pMarketData->BidVolume4)+",";
		insert_sql+=std::to_string(pMarketData->BidPrice5)+",";
		insert_sql+=std::to_string(pMarketData->BidVolume5)+",";
		insert_sql+=std::to_string(pMarketData->AskPrice6)+",";
		insert_sql+=std::to_string(pMarketData->AskVolume6)+",";
		insert_sql+=std::to_string(pMarketData->AskPrice7)+",";
		insert_sql+=std::to_string(pMarketData->AskVolume7)+",";
		insert_sql+=std::to_string(pMarketData->BidPrice6)+",";
		insert_sql+=std::to_string(pMarketData->BidVolume6)+",";
		insert_sql+=std::to_string(pMarketData->BidPrice7)+",";
		insert_sql+=std::to_string(pMarketData->BidVolume7)+",";
		insert_sql+=std::to_string(pMarketData->AskPrice8)+",";
		insert_sql+=std::to_string(pMarketData->AskVolume8)+",";
		insert_sql+=std::to_string(pMarketData->AskPrice9)+",";
		insert_sql+=std::to_string(pMarketData->AskVolume9)+",";
		insert_sql+=std::to_string(pMarketData->BidPrice8)+",";
		insert_sql+=std::to_string(pMarketData->BidVolume8)+",";
		insert_sql+=std::to_string(pMarketData->BidPrice9)+",";
		insert_sql+=std::to_string(pMarketData->BidVolume9)+",";
		insert_sql+=std::to_string(pMarketData->BidPrice10)+",";
		insert_sql+=std::to_string(pMarketData->BidVolume10)+",";
		insert_sql+=std::to_string(pMarketData->AskPrice10)+",";
		insert_sql+=std::to_string(pMarketData->AskVolume10)+",";
		insert_sql+=std::to_string(pMarketData->Info1)+",";
		insert_sql+=std::to_string(pMarketData->Info2)+",";
		insert_sql+=std::to_string(pMarketData->Info3)+",";
		insert_sql+=std::to_string(pMarketData->UpperLimitPrice)+",";
		insert_sql+=std::to_string(pMarketData->LowerLimitPrice)+",";
		insert_sql+=std::to_string(pMarketData->ClosePrice)+",'";
		insert_sql+=std::to_string(pMarketData->MDSecurityStat)+"',";
		insert_sql+=std::to_string(pMarketData->TotalBidNumber)+",";
		insert_sql+=std::to_string(pMarketData->TotalOfferNumber)+",";
		insert_sql+=std::to_string(pMarketData->BidTradeMaxDuration)+",";
		insert_sql+=std::to_string(pMarketData->OfferTradeMaxDuration)+",";
		insert_sql+=std::to_string(pMarketData->IOPV)+",";
		insert_sql+=std::to_string(pMarketData->Ask1NumOrders)+",";
		insert_sql+=std::to_string(pMarketData->Bid1NumOrders)+",";
		insert_sql+=std::to_string(pMarketData->Ask2NumOrders)+",";
		insert_sql+=std::to_string(pMarketData->Bid2NumOrders)+",";
		insert_sql+=std::to_string(pMarketData->Ask3NumOrders)+",";
		insert_sql+=std::to_string(pMarketData->Bid3NumOrders)+",";
		insert_sql+=std::to_string(pMarketData->Ask4NumOrders)+",";
		insert_sql+=std::to_string(pMarketData->Bid4NumOrders)+",";
		insert_sql+=std::to_string(pMarketData->Ask5NumOrders)+",";
		insert_sql+=std::to_string(pMarketData->Bid5NumOrders)+",";
		insert_sql+=std::to_string(pMarketData->Ask6NumOrders)+",";
		insert_sql+=std::to_string(pMarketData->Bid6NumOrders)+",";
		insert_sql+=std::to_string(pMarketData->Ask7NumOrders)+",";
		insert_sql+=std::to_string(pMarketData->Bid7NumOrders)+",";
		insert_sql+=std::to_string(pMarketData->Ask8NumOrders)+",";
		insert_sql+=std::to_string(pMarketData->Bid8NumOrders)+",";
		insert_sql+=std::to_string(pMarketData->Ask9NumOrders)+",";
		insert_sql+=std::to_string(pMarketData->Bid9NumOrders)+",";
		insert_sql+=std::to_string(pMarketData->Ask10NumOrders)+",";
		insert_sql+=std::to_string(pMarketData->Bid10NumOrders)+",";
		insert_sql+=std::to_string(pMarketData->WithdrawBuyNumber)+",";
		insert_sql+=std::to_string(pMarketData->WithdrawBuyAmount)+",";
		insert_sql+=std::to_string(pMarketData->WithdrawBuyMoney)+",";
		insert_sql+=std::to_string(pMarketData->WithdrawSellNumber)+",";
		insert_sql+=std::to_string(pMarketData->WithdrawSellAmount)+",";
		insert_sql+=std::to_string(pMarketData->WithdrawSellMoney)+")";
		marketDataQueue.push(insert_sql);
		if(marketDataQueue.size()>10)ExcuteMarketData();
}