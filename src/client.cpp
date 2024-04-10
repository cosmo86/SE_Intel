#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <random>
//#include <json/json.h>
#include <pthread.h>
#include "/root/vcpkg/packages/jsoncpp_x64-linux/include/json/json.h"
//#include "/root/vcpkg/packages/jsoncpp_x64-linux/lib/libjsoncpp.a"
//#include <jsoncpp/json/json.h>
int sockfd;
class test{
public:
    int a;
    std::string b;
    std::string c;
    double d;
    test(){}
    virtual ~test(){}
    virtual void print(test* k){}
};
class word :public test{
public:
    word(){}
    ~word(){}
    void printf(test*k);
};
void word::printf(test*k){
    std::cout<<k->a<<std::endl;
    std::cout<<k->b<<std::endl;
    std::cout<<k->c<<std::endl;
    std::cout<<k->d<<std::endl;
}
void huidiaoyi(Json::Value ans){
    test T;
    T.a=ans["val"]["a"].asInt();
    T.b=ans["val"]["b"].asString();
    T.c=ans["val"]["c"].asString();
    T.d=ans["val"]["d"].asDouble();
    word C;
    C.printf(&T);
}
void *fun(void *arg){
    while(1){
        char *buff=new char[1024];
         memset(buff,0,sizeof(buff));
         int n=recv(sockfd,buff,1024,0);
         if(n<=0){
            std::cout<<"ser close"<<std::endl;
            break;
        }
        Json::Value val;
        Json::Reader read;
        if(!read.parse(buff,val)){
            std::cout<<"json err"<<std::endl;
        }
        int leixing=val["type"].asInt();
        switch(leixing){
            case 1:huidiaoyi(val);
        }
        
    }
    close(sockfd);
    return nullptr;
}
void sockfd_init(){
    sockfd=socket(AF_INET,SOCK_STREAM,0);//监听套机字
    if(sockfd==-1){
        printf("socket create err");
        exit(0);
    }
    struct sockaddr_in saddr;//代表服务器的地址
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(6000);
    saddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    int res=connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(res==-1){
        printf("connect err\n");
        exit(1);
    }
    pthread_t id;
    pthread_create(&id,NULL,fun,nullptr);
}
int main(){
    sockfd_init();
    while(1){
        sleep(1);
        //std::cout<<"yes"<<std::endl;
        // 使用随机设备作为种子引擎
        std::random_device rd;

        // 使用 Mersenne Twister 引擎，并以随机设备作为种子
        std::mt19937 gen(rd());

        // 定义随机数分布，例如在 [1, 100] 范围内生成整数
        std::uniform_int_distribution<> dis(1, 3);

        // 生成随机数
        int randomNumber = dis(gen);
        Json::Value val;
        val["type"]=randomNumber;
        if(randomNumber==2){
            Json::Value k;
            k["a"]=2;
            k["b"]=2.2;
            k["c"]=std::string("zhangxuanweiwu");
            val["val"]=k;
        }
        
        send(sockfd,val.toStyledString().c_str(),strlen(val.toStyledString().c_str()),0);
    }
}