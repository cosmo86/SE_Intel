#include "../include/L2_quoter.hpp"
#include <vector>
#include <algorithm>
using namespace std;
int main(){
    Lev2MdSpi spi;
    char userid[21],passwd[32],addrs[64];
    strcpy(userid,"00032129");
    strcpy(passwd,"19359120");
    strcpy(addrs,"tcp://210.14.72.17:6900");
    //strcpy(addrs, "tcp://127.0.0.1:8500");
    spi.init(userid,passwd,addrs);
    std::thread websocket(&Lev2MdSpi::Service, &spi);//线程处理网络的连接
    sleep(3);
    std::thread Add(&Lev2MdSpi::Add, &spi);//线程处理增加监听
    std::thread ClickHouse(&Lev2MdSpi::manage_CH, &spi);//线程处理数据库和网络发送
    websocket.join();
    ClickHouse.join();
    Add.join();

    std::cout<<"this ok"<<std::endl;
}
