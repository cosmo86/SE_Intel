#include "../include/L2_quoter.hpp"   
#include <jemalloc/jemalloc.h> 
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
    // 使用 std::bind 创建可调用对象并传递给 std::thread
    std::thread websocket(std::bind(&Lev2MdSpi::init_CH_SV, &spi));//线程处理网络
    //线程处理对应函数。
    std::thread ClickHouse(std::bind(&Lev2MdSpi::manage_CH,&spi));
    //std::thread MarketDate(std::bind(&Lev2MdSpi::manage_MarketDate, &spi));
    //std::thread NGTSTick(std::bind(&Lev2MdSpi::manage_NGTSTick, &spi));
    //std::thread OrderDetail(std::bind(&Lev2MdSpi::manage_OrderDetail, &spi));
    //std::thread Transaction(std::bind(&Lev2MdSpi::manage_Transaction, &spi));
    //主线程阻塞并等待处理
    while(1){}
    std::cout<<"this ok"<<std::endl;
}