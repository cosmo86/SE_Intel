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
    sleep(3);
    spi.add();
    // 使用 std::bind 创建可调用对象并传递给 std::thread
    std::thread websocket(&Lev2MdSpi::init_CH_SV, &spi);//线程处理网络
    //线程处理对应函数。
    std::thread ClickHouse(&Lev2MdSpi::manage_CH, &spi);
    //主线程阻塞并等待处理
    // 获取日志记录器实例

    // 记录不同级别的日志信息

    // 停止日志记录器，并安全地结束线程
    websocket.join();
    ClickHouse.join();

    std::cout<<"this ok"<<std::endl;
}
/*    while(1){
        //cout<<spi.k<<endl;
        if(spi.k>1000){
            std::cout<<"begin()"<<endl;
            const int sum=1000;
            vector<double>buff;
            for(int i=0;i<sum;i++){
                buff.push_back(spi.ans[i]);
            }
            sort(buff.begin(),buff.end());
            std::cout<<1<<endl;
            double ans=0;
            for(int i=0;i<spi.k;i++){
                ans+=buff[i];
            }
            double mean=ans/sum;
            /*logger->info("平均数:",mean);
            logger->info("中位数:",buff[spi.k/2]);
            logger->info("99百分位数:",buff[(int)(spi.k*0.99)]);
           double ans0=0;
           for(int i=0;i<sum;i++){
                ans0+=(buff[i]-mean)*(buff[i]-mean);
           }
           ans0=ans0/(sum-1);
           logger->info("标准差：",ans0);
           break;
        }
    }*/