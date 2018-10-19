#pragma once

#include "BrewBlox.h"
#include "SetpointProfile.h"
#include "blox/Block.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "proto/cpp/SetpointProfile.pb.h"

class SetpointProfileBlock : public Block<blox_SetpointProfile_msgid> {
private:
    SetpointProfile profile;
    using Point = SetpointProfile::Point;

protected:
    static bool streamPointsOut(pb_ostream_t* stream, const pb_field_t* field, void* const* arg)
    {
        const std::vector<Point>* points = reinterpret_cast<std::vector<Point>*>(*arg);
        for (const auto& p : *points) {
            auto submsg = blox_SetpointProfile_Point{p.time, cnl::unwrap(p.temp)};
            if (!pb_encode_tag_for_field(stream, field)) {
                return false;
            }
            if (!pb_encode_submessage(stream, blox_SetpointProfile_Point_fields, &submsg)) {
                return false;
            }
        }
        return true;
    }

    static bool streamPointsIn(pb_istream_t* stream, const pb_field_t* field, void** arg)
    {
        std::vector<Point>* newPoints = reinterpret_cast<std::vector<Point>*>(*arg);

        if (stream->bytes_left) {
            blox_SetpointProfile_Point submsg = blox_SetpointProfile_Point_init_zero;
            if (!pb_decode(stream, blox_SetpointProfile_Point_fields, &submsg)) {
                return false;
            }
            newPoints->push_back(Point{submsg.time, cnl::wrap<decltype(Point::temp)>(submsg.temperature)});
        }
        return true;
    }

public:
    SetpointProfileBlock()
        : profile(bootTimeRef())
    {
    }
    virtual ~SetpointProfileBlock() = default;

    virtual cbox::CboxError streamFrom(cbox::DataIn& in) override final
    {
        blox_SetpointProfile newData;
        std::vector<Point> newPoints;
        newData.points.funcs.decode = &streamPointsIn;
        newData.points.arg = &newPoints;
        cbox::CboxError result = streamProtoFrom(in, &newData, blox_SetpointProfile_fields, std::numeric_limits<size_t>::max() - 1);
        if (result == cbox::CboxError::OK) {
            profile.points(std::move(newPoints));
        }
        return result;
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final
    {
        blox_SetpointProfile message;
        message.points.funcs.encode = &streamPointsOut;
        message.points.arg = const_cast<std::vector<Point>*>(&profile.points());
        cbox::CboxError result = streamProtoTo(out, &message, blox_SetpointProfile_fields, std::numeric_limits<size_t>::max() - 1);
        return result;
    }

    virtual cbox::CboxError streamPersistedTo(cbox::DataOut& out) const override final
    {
        return streamTo(out);
    }

    virtual cbox::update_t update(const cbox::update_t& now) override final
    {
        profile.update(now);
        return update_1s(now);
    }

    virtual void* implements(const cbox::obj_type_t& iface) override final
    {
        if (iface == blox_SetpointProfile_msgid) {
            return this; // me!
        }
        if (iface == cbox::interfaceId<Setpoint>()) {
            // return the member that implements the interface in this case
            Setpoint* ptr = &profile;
            return ptr;
        }
        return nullptr;
    }

    SetpointProfile& get()
    {
        return profile;
    }
};
