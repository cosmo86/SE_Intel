#ifndef SERVICE
#define SERVICE
#include <string>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <event.h>
using namespace std;
class Socket{
    private:
        string m_ips;//ip
        short m_port;//port
        int m_sockfd;//sockfd
    public:
        Socket():m_ips("127.0.0.1"),m_port(6000),m_sockfd(-1){}
        Socket(const string &ips,short port):m_ips(ips),m_port(port),m_sockfd(-1){}
        ~Socket();
    public:
        bool Sockfd_init();
        int Get_Sockfd()const{
            return m_sockfd;
        }
};
class MyLibevent{
public:
    MyLibevent():m_base(NULL){
        m_base=NULL;
    }
    bool MyLibevent_Init(){
        m_base=event_init();
        if(m_base==NULL){
            return false;
        }
        return true;
    }
    /*bool MyLibevent_Add(int fd,Sock_Obj*obj);
    void MyLibevent_Delete(Sock_Obj*obj);*/
    void MyLibevent_Loop(){
        if(m_base!=NULL){
            event_base_dispatch(m_base);
        }
    }
private:
    struct event_base *m_base;
};
#endif