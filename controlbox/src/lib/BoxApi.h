#pragma once

/**
 * Provides an API for interacting with controlbox.
 */

#include "Box.h"
#include "Comms.h"
#include <iostream>
#include <string>
#include "stdarg.h"

class Profile
{
    int8_t id;

public:

    Profile(int8_t id_) : id(id_) {}

    bool is_valid() const
    {
        return id>=0;
    }

    int8_t get_id() const
    {
        return id;
    }

    bool operator==(const Profile& rhs) const
    {
        return rhs.id == id;
    }
};


class IStreamDataIn : public DataIn
{
    std::istream& in;
    uint8_t prev;
public:

    IStreamDataIn(std::istream& in_) : in(in_) {}

    virtual bool hasNext() override {
        return in.peek()!=EOF;
    }

    virtual uint8_t next() override {
        char val;
        in.get(val);
        return uint8_t(val);
    }

    virtual uint8_t peek() override {
        return uint8_t(in.peek());
    }

    virtual unsigned available() override {
        return in.eof() ? 0 : 1;
    }
};


/**
 * Provides a DataOut stream by wrapping a std::ostream.
 */
class OStreamDataOut : public DataOut
{
    std::ostream& out;
public:

    OStreamDataOut(std::ostream& out_) : out(out_) {}

    virtual bool write(uint8_t data) override {
        out.put(data);
        return true;
    }
};



class response_exception : public std::exception {};
class invalid_cmd_response : public response_exception {};
class missing_cmd_response : public response_exception {};
class command_failed : public response_exception {};

class BoxApi
{
    using request_text = std::string;
    using response_text = std::string;

    Box& box;

public:

    BoxApi(Box& box_) : box(box_) {}

    /**
     * Retrieves the active profile.
     * If there is no profile currently active, it returns a Profile whose is_valid() method returns false.
     */
    Profile active_profile()
    {
        int8_t profile_id = int8_t(response(exec("0e")));
        return Profile(profile_id);
    }

    bool activate_profile(Profile& p)
    {
        int8_t error = int8_t(response(exec(format("09 %d", p.get_id()))));
        return !error;
    }

    Profile create_profile()
    {
        int8_t profile_id = int8_t(response(exec("07")));
        return Profile(profile_id);
    }

    void create_object(uint8_t index, uint8_t objtype)
    {
        uint8_t error = (response(exec(format("03 %02x %02x", index, objtype))));
        if (error)
            throw command_failed();
    }

    std::string run_command(std::string cmd)
    {
        return exec(cmd);
    }

protected:

    /**
     * Launches the given command.
     */
    std::string exec(std::string strCmd)
    {
        // convert from a hex text to a binary data stream
        std::stringstream input(strCmd);
        IStreamDataIn dataIn(input);
        TextIn textIn(dataIn);
        HexTextToBinaryIn commandDataIn(textIn);

        // cmdOutput stores raw output bytes (not hex encoded)
        std::stringstream cmdBinaryOutput;
        OStreamDataOut commandDataOut(cmdBinaryOutput);

        box.runCommand(commandDataIn, commandDataOut);

        // the initial part of the response should be the same as the original command.
        // verify they are the same
        std::stringstream strReplay(strCmd);
        IStreamDataIn replayDataIn(strReplay);
        TextIn replayTextIn(replayDataIn);
        HexTextToBinaryIn replayIn(replayTextIn);

        IStreamDataIn cmd(cmdBinaryOutput);
        while (replayIn.hasNext())
        {
            if (!cmd.hasNext())
                throw invalid_cmd_response();
            uint8_t expected = replayIn.next();
            uint8_t actual = cmd.next();
            if (expected!=actual)
                throw new invalid_cmd_response();
        }

        // convert the remaining output to a string.
        std::stringstream strResult;
        OStreamDataOut outResult(strResult);
        BinaryToHexTextOut cmdResult(outResult);

        int length = 0;
        // write the remainder out
        while (cmd.hasNext()) {
            cmdResult.write(cmd.next());
            length++;
        }

        return strResult.str();
    }

    std::string format(const std::string fmt_str, ...) {
        int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
        std::string str;
        std::unique_ptr<char[]> formatted;
        va_list ap;
        while(1) {
            formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
            strcpy(&formatted[0], fmt_str.c_str());
            va_start(ap, fmt_str);
            final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
            va_end(ap);
            if (final_n < 0 || final_n >= n)
                n += abs(final_n - n + 1);
            else
                break;
        }
        return std::string(formatted.get());
    }

    uint8_t response(const std::string& s)
    {
        std::stringstream inString(s);
        IStreamDataIn inData(inString);
        TextIn inText(inData);
        HexTextToBinaryIn inBytes(inText);
        if (inBytes.hasNext())
            return inBytes.next();
        else
            throw new missing_cmd_response();
    }

};
