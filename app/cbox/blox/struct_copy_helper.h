#pragma once
#include "assert_size_helper.h"

/*
 * The proto files compile to .pb.h files defining our message format structs.
 * We try to keep this struct equal to the settings struct in the application class, so we can take a direct copy.
 *
 * Ideally, we would use the same struct definition for both, but our .pb.h files are auto generated from the proto files.
 * Our application classes should also not depend on them. So our Bloc class and App class cannot share the same struct.
 *
 * The compiler doesn't guarantee that 2 strucs with an identical layout are compatible and for example
 * can be memcopied. To ensure a valid deep copy, we can create a union to force equal memory layout and use this
 * union to do copy assignment.
 *
 *
 * We assume that the structs have identical layout, but only assert equal size here. Can we check offsets too?
 * Copy-assignment here creates a deep copy of the original struct and returns it as the new format
 * the compiler will hopefully optimize most of the copying away.
 * We cannot use a memcpy, because the two different structs are not guaranteed to have the same memory alignment.
 */

template<typename NEW, typename ORIG>
NEW copy_struct_as(ORIG const& src){
    assert_size<sizeof(ORIG), sizeof(NEW)>();
    union Helper{
        Helper(ORIG f) : from(f){}
        ORIG from;
        NEW to;
    };
    Helper h(src);
    return h.to;
}

