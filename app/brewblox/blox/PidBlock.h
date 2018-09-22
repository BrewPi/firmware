#pragma once

#include "Pid.h"
#include "ProcessValue.h"
#include "blox/Block.h"
#include "cbox/CboxPtr.h"
#include "proto/cpp/Pid.pb.h"

#include "cbox/CboxPtr.h"

class PidBlock : public Block<blox_Pid_msgid> {
private:
    cbox::CboxPtr<ProcessValue<Pid::in_t>> input;
    cbox::CboxPtr<ProcessValue<Pid::out_t>> output;

    Pid pid;

public:
    PidBlock(cbox::ObjectContainer& objects)
        : input(objects)
        , output(objects)
        , pid(input, output)
    {
    }

    virtual ~PidBlock() = default;

    virtual cbox::CboxError streamFrom(cbox::DataIn& in) override final
    {
        blox_Pid newData;
        cbox::CboxError res = streamProtoFrom(in, &newData, blox_Pid_fields, blox_Pid_size);
        /* if no errors occur, write new settings to wrapped object */
        if (res == cbox::CboxError::OK) {
            input.setId(newData.inputId);
            output.setId(newData.outputId);
            pid.configureFilter(uint8_t(newData.filter), cnl::wrap<Pid::in_t>(newData.filterThreshold));
            pid.enabled(newData.enabled);
            pid.kp(cnl::wrap<Pid::in_t>(newData.kp));
            pid.ti(newData.ti);
            pid.td(newData.td);
        }
        return res;
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final
    {
        blox_Pid message = {0};
        message.inputId = input.getId();
        message.outputId = output.getId();

        message.inputValid = input.valid();
        message.outputValid = output.valid();
        message.inputValue = cnl::unwrap(pid.inputValue());
        message.inputSetting = cnl::unwrap(pid.inputSetting());
        message.outputValue = cnl::unwrap(pid.outputValue());
        message.outputSetting = cnl::unwrap(pid.outputSetting());

        message.filter = blox_Pid_FilterChoice(pid.filterChoice());
        message.filterThreshold = cnl::unwrap(pid.filterThreshold());
        message.enabled = pid.enabled();
        message.active = pid.active();
        message.kp = cnl::unwrap(pid.kp());
        message.ti = pid.ti();
        message.td = pid.td();
        message.p = cnl::unwrap(pid.p());
        message.i = cnl::unwrap(pid.i());
        message.d = cnl::unwrap(pid.d());
        message.error = cnl::unwrap(pid.error());
        message.integral = cnl::unwrap(Pid::out_t(pid.integral()));
        message.derivative = cnl::unwrap(pid.derivative());

        return streamProtoTo(out, &message, blox_Pid_fields, blox_Pid_size);
    }

    virtual cbox::CboxError streamPersistedTo(cbox::DataOut& out) const override final
    {
        blox_Pid message = {0};
        message.inputId = input.getId();
        message.outputId = output.getId();
        message.filter = blox_Pid_FilterChoice(pid.filterChoice());
        message.filterThreshold = cnl::unwrap(pid.filterThreshold());
        message.enabled = pid.enabled();
        message.kp = cnl::unwrap(pid.kp());
        message.ti = pid.ti();
        message.td = pid.td();

        return streamProtoTo(out, &message, blox_Pid_fields, blox_Pid_size);
    }

    virtual cbox::update_t update(const cbox::update_t& now) override final
    {
        pid.update();
        return update_1s(now);
    }

    Pid& get()
    {
        return pid;
    }
};
