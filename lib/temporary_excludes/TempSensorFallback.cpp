/*
 * Copyright 2016 BrewPi/Elco Jacobs.
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

#include "TempSensorFallback.h"

#include "../inc/Logger.h"
#include "LogMessages.h"

temp_t TempSensorFallback::read() const {
    return activeSensor().read();
}

update_t TempSensorFallback::update(const update_t & t) {
    if(main.isConnected()){
        if(onBackupSensor){
            // return to main sensor and log message
            logInfo(BACK_ON_MAIN_SENSOR);
        }
        onBackupSensor = false;
    }
    else{
        if(backup.isConnected()){
            if(!onBackupSensor){
                // this is the first time falling back
                logWarning(FALLING_BACK_ON_BACKUP_SENSOR);
            }
            onBackupSensor = true;
        }
    }
    return t + 1000;
}
