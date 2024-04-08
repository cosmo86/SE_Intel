#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
int main(){
    int sockfd=socket(AF_INET,SOCK_STREAM,0);//监听套机字
    if(sockfd==-1){
        printf("socket create err");
        exit(0);
    }
    //bind()无需绑定，随机分配一个端口，ip地址是固定的
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
    char buff[1024]={0};
    //fgets(buff,128,stdin);
    //send(sockfd,buff,strlen(buff)-1,0);
	while(1){
    memset(buff,0,sizeof(buff));
    recv(sockfd,buff,1024,0);
    printf("buff=%s\n",buff);
    }
    close(sockfd);
}