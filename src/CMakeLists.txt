cmake_minimum_required(VERSION 3.12)
project(main)

set(CMAKE_CXX_STANDARD 17)

# 添加对 OpenSSL 的查找
#find_package(OpenSSL REQUIRED)
#
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz) 
FetchContent_MakeAvailable(json)

# 添加 jemalloc 库的包含目录
include_directories("/root/jemalloc-5.2.0/include")

# 添加 jemalloc 库的链接目录
link_directories("/root/jemalloc-5.2.0/lib")
# 添加子目录
add_subdirectory(/root/clickhouse-app/contribs/clickhouse-cpp zimulu)

# 添加可执行文件及其源文件
add_executable(${PROJECT_NAME} "main.cpp" "L2_quoter.cpp" "ClickHouse.cpp" "service.cpp")

# 添加包含目录
target_include_directories(${PROJECT_NAME} PRIVATE /root/clickhouse-app/contribs/clickhouse-cpp/
                                             /root/clickhouse-app/contribs/clickhouse-cpp/contrib/absl)
# 链接相关库到你的可执行文件
target_link_libraries(${PROJECT_NAME} PRIVATE 
    jemalloc
    clickhouse-cpp-lib 
    /usr/lib64/libdl.so.2
    /root/SE_Intel/lib/liblev2mdapi.so 
    -L/root/vcpkg/packages/jsoncpp_x64-linux/lib -ljsoncpp 
    /root/vcpkg/packages/jsoncpp_x64-linux/lib/libjsoncpp.a 
    -lboost_system #网络
    -lpthread #多线程
    nlohmann_json::nlohmann_json
)
#/usr/lib64/libevent.so 
#/usr/lib64/libevent_pthreads.so 
#OpenSSL::Crypto 
#jemalloc dl
#-ljemalloc -DJEMALLOC_NO_DEMANGLE
#
#/root/SE_Intel/lib/libjemalloc.a
#/root/SE_Intel/lib/libdl.so.2
#-L/root/SE_Intel/lib 
#
   