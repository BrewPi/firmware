/*
 * Copyright 2018 BrewPi B.V.
 *
 * This file is part of Controlbox
 *
 * Controlbox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Controlbox.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "Ticks.h"
#include "blox/Block.h"
#include "cbox/DataStream.h"
#include "cbox/ObjectBase.h"
#include "proto/cpp/Ticks.pb.h"

// provides a protobuf interface to the ticks object
template <typename T>
class TicksBlock : public cbox::ObjectBase<TicksBlock<T>> {
    T& ticks;

public:
    TicksBlock(T& _ticks)
        : ticks(_ticks)
    {
    }
    virtual ~TicksBlock() = default;

    virtual cbox::CboxError streamFrom(cbox::DataIn& dataIn) override final
    {
        blox_Ticks newData;
        cbox::CboxError result = streamProtoFrom(dataIn, &newData, blox_Ticks_fields, blox_Ticks_size);
        if (result == cbox::CboxError::OK) {
            ticks.setNow(newData.secondsSinceEpoch);
        }
        return result;
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final
    {
        blox_Ticks message;
        message.secondsSinceEpoch = ticks.getNow();
        message.millisSinceBoot = ticks.millis();

        return streamProtoTo(out, &message, blox_Ticks_fields, blox_Ticks_size);
    }

    virtual cbox::CboxError streamPersistedTo(cbox::DataOut& out) const override final
    {
        return cbox::CboxError::OK;
    }

    virtual cbox::update_t update(const cbox::update_t& now) override final
    {
        return cbox::Object::update_never(now);
    }

    virtual cbox::obj_type_t typeId() const override final
    {
        return 262;
    }
};