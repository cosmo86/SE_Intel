#include "../include/async_logger.h"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iostream>
#include <fmt/format.h>
#include <pthread.h>

// Helper function to get current timestamp as string
std::string AsyncFileLogger::getCurrentTimestamp() {//获取本地时间
    auto now = std::chrono::system_clock::now();//获取系统时间
    auto time_t_now = std::chrono::system_clock::to_time_t(now);//转化为time_t类型时间
    std::tm tm_now = *std::localtime(&time_t_now);//转化为本地时间
    std::stringstream ss;//字符串流
    ss << std::put_time(&tm_now, "%Y%m%d_%H%M%S");//将时间信息写入字符串流
    return ss.str();//返回流中的字符串
}

AsyncFileLogger::AsyncFileLogger(CusLogger_LogLevel logLevel, CusLogger_LogLevel consoleLevel, int core_id)
    : _logLevel(logLevel), _console_lev(consoleLevel), _is_running(true) {
    std::filesystem::path logDir("Engine_log_cus");//定义日志路径
    if (!std::filesystem::exists(logDir)) {//如果不存在则创建
        std::filesystem::create_directory(logDir);
    }
    _filename = logDir.string() + "/log_" + getCurrentTimestamp() + ".log";//生成日志名称

    // Start the logging thread
    _thread = std::thread(&AsyncFileLogger::processEntries, this);//创建线程来调用processEntries函数
    // Set thread CPU affinity
    if (core_id >= 0) {
        cpu_set_t cpuset;//表示系统中所有的CPU核心的位集合
        CPU_ZERO(&cpuset);//清除，即不选择任何核心
        CPU_SET(core_id, &cpuset);//设置亲和性，将core_id位设置为1
        int rc = pthread_setaffinity_np(_thread.native_handle(), sizeof(cpu_set_t), &cpuset);//将线程_thread设置为在cpuset上指定的核上跑
        if (rc != 0) {//不为0表示失败
            std::cerr << "Error setting thread affinity: " << std::strerror(errno) << std::endl;
        }
    }
}

AsyncFileLogger::~AsyncFileLogger() {
    stop();
}

/*
template<typename... Args>
void AsyncFileLogger::log(CusLogger_LogLevel level, const std::string& format, Args... args) {
    auto message = fmt::format(format, args...);
    _queue.enqueue(std::make_pair(level, message));
}



template<typename... Args>
void AsyncFileLogger::info(const std::string& format, Args... args) {
    log(CusLogger_LogLevel::INFO, format, args...);
}

template<typename... Args>
void AsyncFileLogger::warn(const std::string& format, Args... args) {
    log(CusLogger_LogLevel::WARN, format, args...);
}

template<typename... Args>
void AsyncFileLogger::error(const std::string& format, Args... args) {
    log(CusLogger_LogLevel::ERROR, format, args...);
}

template<typename... Args>
void AsyncFileLogger::debug(const std::string& format, Args... args) {
    log(CusLogger_LogLevel::DEBUG, format, args...);
}
*/

void AsyncFileLogger::stop() {
    _is_running.store(false);//设置store为false
    if (_thread.joinable()) {//等待线程结束
        _thread.join();
    }
    flushQueue();//清空日志队列
}

void AsyncFileLogger::processEntries() {
    std::ofstream ofs(_filename, std::ofstream::out | std::ofstream::app);//创建一个输出文件流，输出和追加
    std::pair<CusLogger_LogLevel, std::string> entry;
    while (_is_running.load()) {//循环执行
        while (_queue.try_dequeue(entry)) {//尝试取出一个日志
            if (entry.first >= _console_lev) {//日志级别是否大于输出
                fmt::print(fg(entry.first >= CusLogger_LogLevel::ERROR ? fmt::color::red : fmt::color::yellow), "{}\n", entry.second);
            }
            ofs << entry.second << std::endl;//写入文件
        }
    }
    ofs.close();//关闭文件
}

void AsyncFileLogger::flushQueue() {//最后执行
    std::ofstream ofs(_filename, std::ofstream::out | std::ofstream::app);
    std::pair<CusLogger_LogLevel, std::string> entry;
    while (_queue.try_dequeue(entry)) {
        if (entry.first >= _console_lev) {
            fmt::print(fg(entry.first >= CusLogger_LogLevel::ERROR ? fmt::color::red : fmt::color::yellow), "{}\n", entry.second);
        }
        ofs << entry.second << std::endl;
    }
    ofs.close();
}



LoggerPtr GetLogger() {
    static LoggerPtr logger = new AsyncFileLogger(CusLogger_LogLevel::DEBUG,CusLogger_LogLevel::WARN,6);
    return logger;
}