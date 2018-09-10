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

#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

class Logger {
public:
    using LogFunctionType = std::function<void(const uint8_t& logLevel, const std::string&)>;
    using Buffer_p = std::unique_ptr<std::ostringstream, std::function<void(std::ostringstream*)>>;

    enum LogLevel : uint8_t {
        DEBUG,
        INFO,
        WARN,
        ERROR
    };

    explicit Logger(LogFunctionType logFunction)
        : m_logFunction(std::move(logFunction))
    {
    }

    Buffer_p operator()(LogLevel e)
    {
        return Buffer_p(new std::ostringstream, [e, this](std::ostringstream* st) {
            m_logFunction(e, st->str());
        });
    }

private:
    LogFunctionType m_logFunction;
};

void
logCerr(const uint8_t& level, const std::string& log)
{
    switch (level) {
    case Logger::DEBUG:
        std::cerr << "DEBUG: " << log;
        break;
    case Logger::INFO:
        std::cerr << "INFO: " << log;
        break;
    case Logger::WARN:
        std::cerr << "DEBUG: " << log;
        break;
    case Logger::ERROR:
        std::cerr << "DEBUG: " << log;
        break;
    }
}

Logger&
testLogger()
{
    static Logger logger(logCerr);
    return logger;
}

Logger& logger = testLogger();

#define LOG_DEBUG *logger(Logger::DEBUG)
#define LOG_INFO *logger(Logger::INFO)
#define LOG_WARN *logger(Logger::WARN)
#define LOG_ERROR *logger(Logger::ERROR)
