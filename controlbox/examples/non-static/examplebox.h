/*
 * Copyright 2016 Matthew McGowan.
 *
 * This file is part of Controlbox.
 *
 * Controlbox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Controlbox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Controlbox.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "../../src/lib/CommsStdIO.h"
#include "Box.h"

#include "Ticks.h"
#include "ArrayEepromAccess.h"
#include "ValueTicks.h"
#include "ValueModels.h"
#include "Values.h"
#include "timems.h"
#include <iostream>
#include <fstream>
#include "stdarg.h"

class SystemTicks : public Ticks
{
public:

    virtual ticks_millis_t millis()
    {
        return millisSinceStartup();
    }
};


class ExampleBox : public CommandCallbacks
{
    bool quit;
    ArrayEepromAccess<1024> eepromAccess;
    SystemTicks ticks;
    StdIOConnection connection;
    Box box;
    std::string eeprom;
    FixedContainer systemRoot;

public:

    ExampleBox(const std::string& eeprom_="") :
            quit(false), box(connection, eepromAccess, ticks, *this, systemRoot), eeprom(eeprom_) {}

    Box& get_box() { return box; }

    enum class object_type : uint8_t {
        ValueTicksScaled = 1
    };

    static constexpr inline uint8_t as_int(object_type t) {
        return static_cast<uint8_t>(t);
    }

    void run()
    {
        while (!should_quit())
        {
            box.loop();
            save_eeprom();
        }
    }

    void save_eeprom()
    {
        if (eepromAccess.hasChanged())
        {
            std::ofstream file;
            file.open(eeprom);
            eepromAccess.save(file);
            file.close();
        }
    }

    void initialize()
    {
        if (!eeprom.empty())
        {
            std::ifstream infile(eeprom);
            if (infile) {
                eepromAccess.load(infile);
                writeAnnotation("loaded eeprom file %s", eeprom.c_str());
                infile.close();
            }
            else {
                std::ofstream outfile(eeprom);
                eepromAccess.save(outfile);
                writeAnnotation("created eeprom file %s", eeprom.c_str());
                outfile.close();
            }
        }
        box.setup();
    }

    void shutdown()
    {
        save_eeprom();
    }

    void writeAnnotation(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        char buf[100];
        vsprintf(buf, fmt, args);
        va_end(args);
        BinaryToHexTextOut out(connection.getDataOut());
        // todo - the connection should support annotations itself for the type of protocol
        out.writeAnnotation(buf);
    }

    /**
     * Application-provided function that creates an object from the object definition.
     */
    virtual Object* createApplicationObject(ObjectDefinition& def, bool dryRun=false)
    {
        uint8_t type = def.type;
        if (dryRun)		// consume the stream without creating an object
            type = 0;

        switch (type)
        {
            case as_int(object_type::ValueTicksScaled):
                return new ScaledTicksValue(ticks);

            default:
                return nullFactory(def);
        }
    }

    /**
     * Function prototype expected by the commands implementation to perform
     * a reset.
     * @param exit false on first call, true on second call. The first call (exit==false) is
     * during command processing, so that actions can be taken before the command response is sent.
     * The second call (exit==true) is called to perform the actual reset.
     */
    virtual void handleReset(bool exit=true)
    {
        quit = true;
    }

    bool should_quit() { return quit; }

    virtual void connectionStarted(StandardConnection& connectino, DataOut& out)
    {
        out.writeAnnotation("Example app at your service.");
    }

    Container& systemRootContainer()
    {
		static Object* systemRootItems[2];
		static FixedContainer systemRoot(sizeof(systemRootItems)/sizeof(systemRootItems[0]), systemRootItems);
		return systemRoot;
    }


    virtual Container* createRootContainer()
    {
        DynamicContainer* d = new DynamicContainer();
        return d;
    }

};

