
/*
 * These functions are copied directly from nanopb pb_encode.c and pb_decode.c
 * Particle includes some functions in their dynamically linked library, but not all.
 * The functions not made available by Particle, but used by our code, are copied here.
 * Omitting this file would result in undefined references for the functions in this file.
 * Including the original pb_encode.c and pb_decode.c would result in duplicate references for
 * the functions in the Particle dynamically linked library.
 */


#include <pb.h>
#include <pb_decode.h>
#include <pb_encode.h>

static bool pb_readbyte(pb_istream_t *stream, pb_byte_t *buf)
{
    if (stream->bytes_left == 0)
        PB_RETURN_ERROR(stream, "end-of-stream");

#ifndef PB_BUFFER_ONLY
    if (!stream->callback(stream, buf, 1))
        PB_RETURN_ERROR(stream, "io error");
#else
    *buf = *(const pb_byte_t*)stream->state;
    stream->state = (pb_byte_t*)stream->state + 1;
#endif

    stream->bytes_left--;

    return true;
}


static bool pb_decode_varint32_eof(pb_istream_t *stream, uint32_t *dest, bool *eof)
{
    pb_byte_t byte;
    uint32_t result;

    if (!pb_readbyte(stream, &byte))
    {
        if (stream->bytes_left == 0)
        {
            if (eof)
            {
                *eof = true;
            }
        }

        return false;
    }

    if ((byte & 0x80) == 0)
    {
        /* Quick case, 1 byte value */
        result = byte;
    }
    else
    {
        /* Multibyte case */
        uint_fast8_t bitpos = 7;
        result = byte & 0x7F;

        do
        {
            if (!pb_readbyte(stream, &byte))
                return false;

            if (bitpos >= 32)
            {
                /* Note: The varint could have trailing 0x80 bytes, or 0xFF for negative. */
                uint8_t sign_extension = (bitpos < 63) ? 0xFF : 0x01;

                if ((byte & 0x7F) != 0x00 && ((result >> 31) == 0 || byte != sign_extension))
                {
                    PB_RETURN_ERROR(stream, "varint overflow");
                }
            }
            else
            {
                result |= (uint32_t)(byte & 0x7F) << bitpos;
            }
            bitpos = (uint_fast8_t)(bitpos + 7);
        } while (byte & 0x80);

        if (bitpos == 35 && (byte & 0x70) != 0)
        {
            /* The last byte was at bitpos=28, so only bottom 4 bits fit. */
            PB_RETURN_ERROR(stream, "varint overflow");
        }
   }

   *dest = result;
   return true;
}


bool pb_decode_varint32(pb_istream_t *stream, uint32_t *dest)
{
    return pb_decode_varint32_eof(stream, dest, NULL);
}


bool pb_make_string_substream(pb_istream_t *stream, pb_istream_t *substream)
{
    uint32_t size;
    if (!pb_decode_varint32(stream, &size))
        return false;

    *substream = *stream;
    if (substream->bytes_left < size)
        PB_RETURN_ERROR(stream, "parent stream too short");

    substream->bytes_left = size;
    stream->bytes_left -= size;
    return true;
}

bool pb_close_string_substream(pb_istream_t *stream, pb_istream_t *substream)
{
    if (substream->bytes_left) {
        if (!pb_read(substream, NULL, substream->bytes_left))
            return false;
    }

    stream->state = substream->state;

#ifndef PB_NO_ERRMSG
    stream->errmsg = substream->errmsg;
#endif
    return true;
}


bool pb_encode_delimited(pb_ostream_t *stream, const pb_field_t fields[], const void *src_struct)
{
    return pb_encode_submessage(stream, fields, src_struct);
}


bool pb_decode_delimited_noinit(pb_istream_t *stream, const pb_field_t fields[], void *dest_struct)
{
    pb_istream_t substream;
    bool status;

    if (!pb_make_string_substream(stream, &substream))
        return false;

    status = pb_decode_noinit(&substream, fields, dest_struct);

    if (!pb_close_string_substream(stream, &substream))
        return false;
    return status;
}
