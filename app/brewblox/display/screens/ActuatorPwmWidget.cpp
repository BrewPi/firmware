/*
 * Copyright 2018 BrewPi B.V.
 *
 * This file is part of BrewPi.
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

#include "ActuatorPwmWidget.h"
#include "BrewBlox.h"
#include "FixedPoint.h"

ActuatorPwmWidget::ActuatorPwmWidget(WidgetWrapper& myWrapper, const cbox::obj_id_t& id)
    : ProcessValueWidgetBase(myWrapper)
    , lookup(brewbloxBox().makeCboxPtr<ActuatorPwm>(id))
{
    setClickHandler(this, onClickStatic);
}

void
ActuatorPwmWidget::update()
{
    if (auto ptr = lookup.const_lock()) {
        setConnected();
        char buf[12];
        to_chars_dec(ptr->value(), buf, 12, 1);
        setValue(buf);
        to_chars_dec(ptr->setting(), buf, 12, 1);
        setSetting(buf);

        switch (ptr->targetState()) {
        case ActuatorPwm::State::Inactive:
            buf[0] = 0x26;
            break;
        case ActuatorPwm::State::Active:
            buf[0] = 0x27;
            break;
        default:
            buf[0] = 0x28;
            break;
        }
        buf[1] = 0;
        setIcons(buf);
        return;
    }
    setDisconnected();
}
