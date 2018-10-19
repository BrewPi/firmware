#define CATCH_CONFIG_MAIN

#include "Logger.h"
#include <catch.hpp>
#include <iostream>

Logger&
logger()
{
    static auto logger = Logger([](Logger::LogLevel level, const std::string& log) {
        std::cerr << "LOG (";
        switch (level) {
        case Logger::LogLevel::DEBUG:
            std::cerr << "DEBUG): ";
            break;
        case Logger::LogLevel::INFO:
            std::cerr << "INFO): ";
            break;
        case Logger::LogLevel::WARN:
            std::cerr << "WARN): ";
            break;
        case Logger::LogLevel::ERROR:
            std::cerr << "ERROR): ";
            break;
        }
        std::cerr << log << std::endl;
    });
    return logger;
}