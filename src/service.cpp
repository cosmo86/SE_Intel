#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <event.h>
int socket_init();
void accept_c(int c,short ev,void *arg){
    if(ev&EV_READ){
    char buff[128]={0};
    int n=recv(c,buff,127,0);
    if(n<=0){
        close(c);
        //event_free();
		return;
    }
    else{
        printf("%d is buff=%s\n",c,buff);
        send(c,"ok",2,0);
    }}
}
void accept_cb(int sockfd,short ev,void *arg){
    if(ev&EV_READ){//如果读事件
		int c=accept(sockfd,NULL,NULL);
        if(c<0){
            return;
        }
        printf("c=%d",c);
        struct event *c_cv=event_new((struct event_base*)arg,c,EV_READ|EV_PERSIST,accept_c,NULL);
        while(1){
            send(c,"ok",2,0);
        }
        event_add(c_cv,NULL);
    }
}
int main(){
    int sockfd=socket_init();
    if(sockfd==-1){
        exit(1);
    }
   	struct event_base *base=event_init();
    if(base==NULL){
		exit(1);
    }
    struct event*sock_ev=event_new(base,sockfd,EV_READ|EV_PERSIST,accept_cb,base);
    event_add(sock_ev,NULL);
    event_base_dispatch(base);
    event_free(sock_ev);
    event_base_free(base);
}
int socket_init(){
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1){
        printf("socket err\n");
        return -1;
    }
    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(6000);
    saddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    int res=bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(res==-1){
        printf("bind err\n");
        return -1;
    }
    res=listen(sockfd,5);
    if(res==-1){
        printf("listen err\n");
        return -1;
    }
    return sockfd;
}