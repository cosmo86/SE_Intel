#include "../include/L2_quoter.hpp"   
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
    spi.init_CH_SV();

    while(1){}
    std::cout<<"this ok"<<std::endl;
}