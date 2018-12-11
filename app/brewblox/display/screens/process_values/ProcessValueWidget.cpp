/*
 * Copyright 2015 BrewPi / Elco Jacobs, Matthew McGowan.
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

#include "ProcessValueWidget.h"
#include "ProcessValue.h"
#include "Temperature.h"

void
ProcessValueWidget::setEnabled(bool enabled)
{
    D4D_EnableObject(pObject, enabled);
    D4D_EnableObject(D4D_PV_VALUE(pObject), enabled);
    D4D_EnableObject(D4D_PV_SETTING(pObject), enabled);
    D4D_EnableObject(D4D_PV_NAME(pObject), enabled);
}

void
ProcessValueWidget::update()
{
    using namespace cnl;
    if (auto pv = pvLookup.const_lock()) {
        D4D_SetText(pObject->pRelations[1], "20.0");
        D4D_SetText(pObject->pRelations[2], "21.0");
        D4D_SetText(pObject->pRelations[3], "0123456789012345");
        setEnabled(true);
        return;
    }
    setEnabled(false);
}
