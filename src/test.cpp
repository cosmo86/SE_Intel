#include "../include/async_logger.h"
#include <iostream>

int main() {
    // 获取日志记录器实例
    LoggerPtr logger = GetLogger();

    // 记录不同级别的日志信息
    logger->info("This is an info level message.");
    logger->warn("This is a warning level message.");
    logger->error("This is an error level message.");
    logger->debug("This is a debug level message.");

    // 模拟一些工作，可能产生更多日志
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 停止日志记录器，并安全地结束线程
    logger->stop();

    return 0;
}