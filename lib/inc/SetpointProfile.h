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

#include "Setpoint.h"
#include "Temperature.h"
#include "TicksTypes.h"
#include <algorithm>
#include <vector>

class SetpointProfile : public Setpoint {
public:
    struct Point {
        ticks_seconds_t time;
        temp_t temp;
    };

private:
    temp_t m_current = 0;
    bool m_valid = false;
    ticks_seconds_t m_deviceStartTime = 0;

    std::vector<Point> m_points;

public:
    SetpointProfile()
    {
    }

    void addPoint(Point&& p)
    {
        m_points.push_back(std::move(p));
    }

    void removeAllPoints()
    {
        m_points.clear();
    }

    ticks_seconds_t deviceStartTime() const
    {
        return m_deviceStartTime;
    }

    void deviceStartTime(const ticks_seconds_t& start)
    {
        m_deviceStartTime = start;
    }

    void update(const ticks_millis_t& now)
    {
        struct TimeStampLess {
            bool operator()(const Point& p, const ticks_seconds_t& time) const { return p.time < time; }
            bool operator()(const ticks_seconds_t& time, const Point& p) const { return time < p.time; }
        };

        auto nowSeconds = ticks_seconds_t(now / 1000) + m_deviceStartTime;
        auto upper = std::lower_bound(m_points.cbegin(), m_points.cend(), nowSeconds, TimeStampLess{});
        if (m_points.empty() || m_deviceStartTime == 0) {
            m_current = 0;
            m_valid = false;
            return;
        }
        if (upper == m_points.cbegin()) {
            m_current = m_points.front().temp;
            m_valid = false;
            return;
        }
        if (upper == m_points.cend()) {
            m_current = m_points.back().temp;
            m_valid = false;
            return;
        }
        auto lower = upper - 1;

        auto segmentElapsed = nowSeconds - lower->time;
        auto segmentDuration = upper->time - lower->time;
        auto interpolated = lower->temp + segmentElapsed * (upper->temp - lower->temp) / (segmentDuration);
        m_current = interpolated;
        m_valid = true;
    }

    virtual temp_t
    setting() const override final
    {
        return m_current;
    }

    virtual void
    setting(const temp_t& val) override final
    {
        // setting cannot be set using Setpoint base class
    }

    virtual bool
    valid() override final
    {
        return m_valid;
    }

    virtual void
    valid(bool v) override final
    {
        // valid cannot be set using Setpoint base class
    }

    const std::vector<Point>& points() const
    {
        return m_points;
    }

    void points(std::vector<Point>&& newPoints)
    {
        m_points = newPoints;
    }
};
