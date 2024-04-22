cmake_minimum_required(VERSION 3.12)
project(main)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)


add_subdirectory(/root/clickhouse-app/contribs/clickhouse-cpp zimulu)

add_executable(${PROJECT_NAME} "main.cpp" "L2_quoter.cpp" "ClickHouse.cpp" "service.cpp")

target_include_directories(${PROJECT_NAME} PRIVATE /root/clickhouse-app/contribs/clickhouse-cpp/ /root/clickhouse-app/contribs/clickhouse-cpp/contrib/absl)

link_directories(/root/SE_Intel/lib)

target_link_libraries(${PROJECT_NAME} PRIVATE clickhouse-cpp-lib /root/SE_Intel/lib/liblev2mdapi.so /usr/lib64/libevent.so /usr/lib64/libevent_pthreads.so /root/vcpkg/packages/jsoncpp_x64-linux/lib/libjsoncpp.a OpenSSL::Crypto -lboost_system -lpthread -L/root/vcpkg/packages/jsoncpp_x64-linux/lib -ljsoncpp)