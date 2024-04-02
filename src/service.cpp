#include "../include/service.hpp"
bool Socket:: Sockfd_init(){
    m_sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(m_sockfd==-1){
        return false;
    }
    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(m_port);
    saddr.sin_addr.s_addr=inet_addr(m_ips.c_str());
    int res=bind(m_sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(res<0)return false;
    res=listen(m_sockfd,5);//最多监听五个
    if(res<0)return false;
    return true;
}