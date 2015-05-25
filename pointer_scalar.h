
#ifndef POINTER_SCALAR_H
#define	POINTER_SCALAR_H

#ifdef	__cplusplus
extern "C" {
#endif


/**
 * The scalar type that is the same size as a pointer.
 */
#ifdef ARDUINO
typedef uint16_t pointer_scalar;
#elif __x86_64__ || __ppc64__
typedef uint64_t pointer_scalar;
#else
typedef uint32_t pointer_scalar;
#endif

#define PLATFORM_BIG_ENDIAN 0

// todo - move to platform?

#ifdef	__cplusplus
}
#endif

#endif	/* POINTER_SCALAR_H */

