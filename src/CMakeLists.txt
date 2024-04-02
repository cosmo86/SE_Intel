cmake_minimum_required(VERSION 3.12)
project(main)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(/root/clickhouse-app/contribs/clickhouse-cpp zimulu)

add_executable(${PROJECT_NAME} "L2_quoter.cpp")

target_include_directories(${PROJECT_NAME} PRIVATE /root/clickhouse-app/contribs/clickhouse-cpp/ /root/clickhouse-app/contribs/clickhouse-cpp/contrib/absl)

link_directories(/root/SE_Intel/lib)

target_link_libraries(${PROJECT_NAME} PRIVATE clickhouse-cpp-lib /root/SE_Intel/lib/liblev2mdapi.so)