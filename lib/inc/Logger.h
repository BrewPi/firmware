/*
 * Copyright 2018 BrewPi B.V.
 *
 * This file is part of the BrewBlox Control Library.
 *
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "string_operators.h"
#include <cstdint>
#include <functional>
#include <memory>
#include <string>

class Logger {
public:
    enum LogLevel : uint8_t {
        DEBUG,
        INFO,
        WARN,
        ERROR
    };

    using LogWriteFunction = std::function<void(const LogLevel& logLevel, const std::string&)>;
    using StringBuffer = std::unique_ptr<std::string, std::function<void(std::string*)>>;

    explicit Logger(LogWriteFunction&& logFunction)
        : m_logWriteFunction(logFunction)
    {
    }

    StringBuffer operator()(LogLevel e, const char* initStr)
    {
        return StringBuffer(new std::string(initStr), [e, this](std::string* st) {
            m_logWriteFunction(e, *st);
            delete st;
        });
    }

private:
    LogWriteFunction m_logWriteFunction;
};

extern Logger&
logger();

#define CL_LOG_DEBUG(initStr) *logger()(Logger::DEBUG, initStr)
#define CL_LOG_INFO(initStr) *logger()(Logger::INFO, initStr)
#define CL_LOG_WARN(initStr) *logger()(Logger::WARN, initStr)
#define CL_LOG_ERROR(initStr) *logger()(Logger::ERROR, initStr)
