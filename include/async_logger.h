#pragma once

#include <string>
#include <thread>
#include <atomic>
#include <fstream>
#include <filesystem>
#include <iomanip> 
#include "concurrentqueue.h"
#include "Event.h"
#include <fmt/ostream.h>
#include <fmt/color.h>
#include <fmt/format.h>
enum class CusLogger_LogLevel {//类型说明类
    DEBUG,
    INFO,
    WARN,
    ERROR
};
template<>
struct fmt::formatter<Eventtype> : fmt::formatter<std::string> {//将枚举类型转化为字符串
    // Use the formatter for std::string to convert enum to string
    template<typename FormatContext>
    auto format(const Eventtype& e, FormatContext& ctx) -> decltype(ctx.out()) {
        std::string name = "Unknown";
        switch (e) {
            case Eventtype::TICK: name = "TICK"; break;
            case Eventtype::ORDER: name = "ORDER"; break;
            case Eventtype::ORDER_SUCCESS: name = "ORDER_SUCCESS"; break;
            case Eventtype::ORDER_ERROR: name = "ORDER_ERROR"; break;
            case Eventtype::CANCEL_SUCCESS: name = "CANCEL_SUCCESS"; break;
            case Eventtype::CANCEL_ERROR: name = "CANCEL_ERROR"; break;
            case Eventtype::TRADE: name = "TRADE"; break;
            case Eventtype::L2TICK: name = "L2TICK"; break;
            case Eventtype::ORDER_DETIAL: name = "ORDER_DETAIL"; break;
            case Eventtype::TRANSACTION: name = "TRANSACTION"; break;
            case Eventtype::NGTSTICK: name = "NGTSTICK"; break;
            default: break;  // Default already set to "Unknown"
        }
        return fmt::formatter<std::string>::format(name, ctx);
    }
};
class AsyncFileLogger {
public:
    AsyncFileLogger(CusLogger_LogLevel logLevel = CusLogger_LogLevel::INFO, CusLogger_LogLevel consoleLevel = CusLogger_LogLevel::WARN, int core_id = 6);
    ~AsyncFileLogger();

    template<typename... Args>
    void log(CusLogger_LogLevel level, const std::string& format, Args... args)
    {
        auto message = fmt::format(format, args...);//格式化字符串
        _queue.enqueue(std::make_pair(level, message));//存储日志信息
    }

    template<typename... Args>
    void info(const std::string& format, Args... args)
    {
        log(CusLogger_LogLevel::INFO, format, args...);
    }
    template<typename... Args>
    void warn(const std::string& format, Args... args)
    {
        log(CusLogger_LogLevel::WARN, format, args...);
    };
    template<typename... Args>
    void error(const std::string& format, Args... args)
    {
        log(CusLogger_LogLevel::ERROR, format, args...);
    }
    template<typename... Args>
    void debug(const std::string& format, Args... args)
    {
        log(CusLogger_LogLevel::DEBUG, format, args...);
    }

    void stop();

private:
    void processEntries();
    void flushQueue();
    std::string getCurrentTimestamp();//获取时间

    std::string _filename;//日志文件名
    std::thread _thread;//日志记录线程
    std::atomic<bool> _is_running;//bool类型的原子变量，用于控制日志记录线程的运行状态
    moodycamel::ConcurrentQueue<std::pair<CusLogger_LogLevel, std::string>> _queue;//线程安全的队列
    CusLogger_LogLevel _logLevel;//日志级别
    CusLogger_LogLevel _console_lev;//输出的日志级别
};

using LoggerPtr = AsyncFileLogger*;

LoggerPtr GetLogger();