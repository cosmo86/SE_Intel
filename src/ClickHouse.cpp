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
    sql+=std::string("(ExchangeID String,SecurityID String,TradeTime Int32,TradePrice Float64,TradeVolume Int64,ExecType String,MainSeq Int32,SubSeq Int64,BuyNo Int64,SellNo Int64,Info1 Int32,Info2 Int32,Info3 Int32,TradeBSFlag String,BizIndex Int64) ENGINE = MergeTree() ORDER BY (TradeTime, MainSeq, SubSeq)");            std::cout<<sql<<std::endl;
    client.Execute(sql.c_str());//建表
}
void ClickHouse::buildOrderDetail(){
    std::string sql="CREATE TABLE IF NOT EXISTS OrderDetail_";
    sql+=getCurrentDate();
    sql+=std::string("(ExchangeID String,SecurityID String,OrderTime Int32,Price Float64,Volume Int64,Side String,OrderType String,MainSeq Int32,SubSeq Int32,Info1 Int32,Info2 Int32,Info3 Int32,OrderNO Int64,OrderStatus String,BizIndex Int64) ENGINE = MergeTree() ORDER BY (OrderTime, ExchangeID, SecurityID);");            std::cout<<sql<<std::endl;
    client.Execute(sql.c_str());//建表
}
void ClickHouse::execute(std::string sql){
    client.Execute(sql.c_str());
}
void ClickHouse::insertTransaction(CTORATstpLev2TransactionField* pTransaction){
    std::string sql = "INSERT INTO TransactionField_" + getCurrentDate() + " (ExchangeID, SecurityID, TradeTime, TradePrice, TradeVolume, ExecType, MainSeq, SubSeq, BuyNo, SellNo, Info1, Info2, Info3, TradeBSFlag, BizIndex) VALUES ";
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
}
void ClickHouse::insertOrderDetail(CTORATstpLev2OrderDetailField* pOrderDetail){
    std::string sql = "INSERT INTO OrderDetail_" + getCurrentDate() + " (ExchangeID,SecurityID,OrderTime,Price,Volume,Side,OrderType,MainSeq,SubSeq,Info1,Info2,Info3,OrderNO,OrderStatus,BizIndex) VALUES ";
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
void ClickHouse::insertNGTSTick(CTORATstpLev2NGTSTickField* pTick){
    std::string sql_insert = "INSERT INTO NGTSTickField_";
            sql_insert += getCurrentDate(); // Assuming gettime() returns the current time or some unique identifier
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