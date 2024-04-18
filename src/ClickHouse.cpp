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
void ClickHouse::Insert(std::vector<void *>&it,int sign){}