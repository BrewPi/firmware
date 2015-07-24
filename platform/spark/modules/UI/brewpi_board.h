
#ifndef BREWPI_BOARD_H
#define	BREWPI_BOARD_H

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

bool IsBoardRevC();

#define MAX_ACTUATOR_COUNT (4)

#ifdef __cplusplus
}
#endif


#endif	/* BREWPI_BOARD_H */

