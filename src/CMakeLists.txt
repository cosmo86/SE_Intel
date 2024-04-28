cmake_minimum_required(VERSION 3.12)
project(main)

set(CMAKE_CXX_STANDARD 17)

# 添加对 OpenSSL 的查找
find_package(OpenSSL REQUIRED)

# 添加对 concurrentqueue 的查找
#find_package(unofficial-concurrentqueue CONFIG REQUIRED)

# 添加子目录
add_subdirectory(/root/clickhouse-app/contribs/clickhouse-cpp zimulu)

# 添加可执行文件及其源文件
add_executable(${PROJECT_NAME} "main.cpp" "L2_quoter.cpp" "ClickHouse.cpp" "service.cpp")

# 添加包含目录
target_include_directories(${PROJECT_NAME} PRIVATE /root/clickhouse-app/contribs/clickhouse-cpp/
                                             /root/clickhouse-app/contribs/clickhouse-cpp/contrib/absl)


# 链接相关库到你的可执行文件
target_link_libraries(${PROJECT_NAME} PRIVATE 
    clickhouse-cpp-lib 
    /root/SE_Intel/lib/liblev2mdapi.so 
    /usr/lib64/libevent.so 
    /usr/lib64/libevent_pthreads.so 
    /root/vcpkg/packages/jsoncpp_x64-linux/lib/libjsoncpp.a 
    #/root/jemalloc/lib/libjemalloc.a
    OpenSSL::Crypto 
    -lboost_system 
    -lpthread 
    -L/root/vcpkg/packages/jsoncpp_x64-linux/lib 
    -ljsoncpp
    #-L/root/jemalloc/lib
    #unofficial::concurrentqueue::concurrentqueue  # 链接 concurrentqueue
   
)