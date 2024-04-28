#include <stdlib.h>
#include <atomic>
#include <iostream>
//#include "TORATstpLev2ApiStruct.h"
//#include "TORATstpLev2ApiDataType.h"
//#include "TORATstpLev2MdApi.h"
using namespace std;
template<typename _T>
class memory_pool{
private:
    _T *pool;
    int size;
public:
    std::atomic<int>first;
    std::atomic<int>last;
    int length()const{return size;}
    memory_pool():size(0),first(0),last(-1){}
    ~memory_pool(){
        free(pool);
    }
    void Init(int len){
        size=len;
        pool=(_T*)malloc(sizeof(_T)*size);
    }
    _T& operator[](int x){
        //assert(x>=0 && x<=size-1);
        return pool[x];
    }
};