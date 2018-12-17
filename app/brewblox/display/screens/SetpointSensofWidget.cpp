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

#include "BrewBlox.h"
#include "SetpointSensorWidget.h"

SetpointSensorWidget::SetpointSensorWidget(WidgetWrapper& myWrapper, const cbox::obj_id_t& id)
    : ProcessValueWidgetBase(myWrapper)
    , lookup(brewbloxBox().makeCboxPtr<SetpointSensorPair>(id))
{
    setClickHandler(this, onClickStatic);
}

void
SetpointSensorWidget::update()
{
    if (auto ptr = lookup.const_lock()) {
        setConnected();

        char icons[3] = {0};
        if (ptr->sensorValid()) {
            setValue(temp_to_string(ptr->value(), 1).c_str());
            icons[0] = '\x29';
        } else {
            setValue(nullptr);
            icons[0] = '\x2B';
        }
        if (ptr->setpointValid()) {
            setSetting(temp_to_string(ptr->setting(), 1).c_str());
            icons[1] = '\x2A';
        } else {
            setSetting(nullptr);
        }
        setIcons(icons);
        return;
    }
    setDisconnected();
}
