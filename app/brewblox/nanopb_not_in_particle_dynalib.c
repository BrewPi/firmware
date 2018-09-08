
/*
 * These functions are copied directly from nanopb pb_encode.c and pb_decode.c
 * Particle includes some functions in their dynamically linked library, but not all.
 * The functions not made available by Particle, but used by our code, are copied here.
 * Omitting this file would result in undefined references for the functions in this file.
 * Including the original pb_encode.c and pb_decode.c would result in duplicate references for
 * the functions in the Particle dynamically linked library.
 */

#include <pb.h>
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>

bool
pb_decode_fixed64(pb_istream_t* stream, void* dest);
bool
pb_encode_fixed64(pb_ostream_t* stream, const void* value);

bool
pb_write(pb_ostream_t* stream, const pb_byte_t* buf, size_t count);
//bool
//#pb_field_iter_begin(pb_field_iter_t* iter, const pb_field_t* fields, void* dest_struct);
//bool
//pb_field_iter_next(pb_field_iter_t* iter);
static void
pb_message_set_to_defaults(const pb_field_t fields[], void* dest_struct);
bool
pb_decode(pb_istream_t* stream, const pb_field_t fields[], void* dest_struct);
static void
pb_field_set_to_default(pb_field_iter_t* iter);
static void
iter_from_extension(pb_field_iter_t* iter, pb_extension_t* extension);

bool
pb_decode_fixed64(pb_istream_t* stream, void* dest)
{
    pb_byte_t bytes[8];

    if (!pb_read(stream, bytes, 8))
        return false;

    *(uint64_t*)dest = ((uint64_t)bytes[0] << 0) | ((uint64_t)bytes[1] << 8) | ((uint64_t)bytes[2] << 16) | ((uint64_t)bytes[3] << 24) | ((uint64_t)bytes[4] << 32) | ((uint64_t)bytes[5] << 40) | ((uint64_t)bytes[6] << 48) | ((uint64_t)bytes[7] << 56);

    return true;
}

bool
pb_encode_fixed64(pb_ostream_t* stream, const void* value)
{
    uint64_t val = *(const uint64_t*)value;
    pb_byte_t bytes[8];
    bytes[0] = (pb_byte_t)(val & 0xFF);
    bytes[1] = (pb_byte_t)((val >> 8) & 0xFF);
    bytes[2] = (pb_byte_t)((val >> 16) & 0xFF);
    bytes[3] = (pb_byte_t)((val >> 24) & 0xFF);
    bytes[4] = (pb_byte_t)((val >> 32) & 0xFF);
    bytes[5] = (pb_byte_t)((val >> 40) & 0xFF);
    bytes[6] = (pb_byte_t)((val >> 48) & 0xFF);
    bytes[7] = (pb_byte_t)((val >> 56) & 0xFF);
    return pb_write(stream, bytes, 8);
}

bool
pb_write(pb_ostream_t* stream, const pb_byte_t* buf, size_t count)
{
    if (stream->callback != NULL) {
        if (stream->bytes_written + count > stream->max_size)
            PB_RETURN_ERROR(stream, "stream full");

#ifdef PB_BUFFER_ONLY
        if (!buf_write(stream, buf, count))
            PB_RETURN_ERROR(stream, "io error");
#else
        if (!stream->callback(stream, buf, count))
            PB_RETURN_ERROR(stream, "io error");
#endif
    }

    stream->bytes_written += count;
    return true;
}

bool
pb_field_iter_begin(pb_field_iter_t* iter, const pb_field_t* fields, void* dest_struct)
{
    iter->start = fields;
    iter->pos = fields;
    iter->required_field_index = 0;
    iter->dest_struct = dest_struct;
    iter->pData = (char*)dest_struct + iter->pos->data_offset;
    iter->pSize = (char*)iter->pData + iter->pos->size_offset;

    return (iter->pos->tag != 0);
}

bool
pb_field_iter_next(pb_field_iter_t* iter)
{
    const pb_field_t* prev_field = iter->pos;

    if (prev_field->tag == 0) {
        /* Handle empty message types, where the first field is already the terminator.
         * In other cases, the iter->pos never points to the terminator. */
        return false;
    }

    iter->pos++;

    if (iter->pos->tag == 0) {
        /* Wrapped back to beginning, reinitialize */
        (void)pb_field_iter_begin(iter, iter->start, iter->dest_struct);
        return false;
    } else {
        /* Increment the pointers based on previous field size */
        size_t prev_size = prev_field->data_size;

        if (PB_HTYPE(prev_field->type) == PB_HTYPE_ONEOF && PB_HTYPE(iter->pos->type) == PB_HTYPE_ONEOF && iter->pos->data_offset == PB_SIZE_MAX) {
            /* Don't advance pointers inside unions */
            return true;
        } else if (PB_ATYPE(prev_field->type) == PB_ATYPE_STATIC && PB_HTYPE(prev_field->type) == PB_HTYPE_REPEATED) {
            /* In static arrays, the data_size tells the size of a single entry and
             * array_size is the number of entries */
            prev_size *= prev_field->array_size;
        } else if (PB_ATYPE(prev_field->type) == PB_ATYPE_POINTER) {
            /* Pointer fields always have a constant size in the main structure.
             * The data_size only applies to the dynamically allocated area. */
            prev_size = sizeof(void*);
        }

        if (PB_HTYPE(prev_field->type) == PB_HTYPE_REQUIRED) {
            /* Count the required fields, in order to check their presence in the
             * decoder. */
            iter->required_field_index++;
        }

        iter->pData = (char*)iter->pData + prev_size + iter->pos->data_offset;
        iter->pSize = (char*)iter->pData + iter->pos->size_offset;
        return true;
    }
}

static void
pb_message_set_to_defaults(const pb_field_t fields[], void* dest_struct)
{
    pb_field_iter_t iter;

    if (!pb_field_iter_begin(&iter, fields, dest_struct))
        return; /* Empty message type */

    do {
        pb_field_set_to_default(&iter);
    } while (pb_field_iter_next(&iter));
}

bool
pb_decode(pb_istream_t* stream, const pb_field_t fields[], void* dest_struct)
{
    bool status;
    pb_message_set_to_defaults(fields, dest_struct);
    status = pb_decode_noinit(stream, fields, dest_struct);

#ifdef PB_ENABLE_MALLOC
    if (!status)
        pb_release(fields, dest_struct);
#endif

    return status;
}

/* Initialize message fields to default values, recursively */
static void
pb_field_set_to_default(pb_field_iter_t* iter)
{
    pb_type_t type;
    type = iter->pos->type;

    if (PB_LTYPE(type) == PB_LTYPE_EXTENSION) {
        pb_extension_t* ext = *(pb_extension_t* const*)iter->pData;
        while (ext != NULL) {
            pb_field_iter_t ext_iter;
            ext->found = false;
            iter_from_extension(&ext_iter, ext);
            pb_field_set_to_default(&ext_iter);
            ext = ext->next;
        }
    } else if (PB_ATYPE(type) == PB_ATYPE_STATIC) {
        bool init_data = true;
        if (PB_HTYPE(type) == PB_HTYPE_OPTIONAL && iter->pSize != iter->pData) {
            /* Set has_field to false. Still initialize the optional field
             * itself also. */
            *(bool*)iter->pSize = false;
        } else if (PB_HTYPE(type) == PB_HTYPE_REPEATED || PB_HTYPE(type) == PB_HTYPE_ONEOF) {
            /* REPEATED: Set array count to 0, no need to initialize contents.
               ONEOF: Set which_field to 0. */
            *(pb_size_t*)iter->pSize = 0;
            init_data = false;
        }

        if (init_data) {
            if (PB_LTYPE(iter->pos->type) == PB_LTYPE_SUBMESSAGE) {
                /* Initialize submessage to defaults */
                pb_message_set_to_defaults((const pb_field_t*)iter->pos->ptr, iter->pData);
            } else if (iter->pos->ptr != NULL) {
                /* Initialize to default value */
                memcpy(iter->pData, iter->pos->ptr, iter->pos->data_size);
            } else {
                /* Initialize to zeros */
                memset(iter->pData, 0, iter->pos->data_size);
            }
        }
    } else if (PB_ATYPE(type) == PB_ATYPE_POINTER) {
        /* Initialize the pointer to NULL. */
        *(void**)iter->pData = NULL;

        /* Initialize array count to 0. */
        if (PB_HTYPE(type) == PB_HTYPE_REPEATED || PB_HTYPE(type) == PB_HTYPE_ONEOF) {
            *(pb_size_t*)iter->pSize = 0;
        }
    } else if (PB_ATYPE(type) == PB_ATYPE_CALLBACK) {
        /* Don't overwrite callback */
    }
}

static void
iter_from_extension(pb_field_iter_t* iter, pb_extension_t* extension)
{
    /* Fake a field iterator for the extension field.
     * It is not actually safe to advance this iterator, but decode_field
     * will not even try to. */
    const pb_field_t* field = (const pb_field_t*)extension->type->arg;
    (void)pb_field_iter_begin(iter, field, extension->dest);
    iter->pData = extension->dest;
    iter->pSize = &extension->found;

    if (PB_ATYPE(field->type) == PB_ATYPE_POINTER) {
        /* For pointer extensions, the pointer is stored directly
         * in the extension structure. This avoids having an extra
         * indirection. */
        iter->pData = &extension->dest;
    }
}
