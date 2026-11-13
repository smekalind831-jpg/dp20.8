#ifndef CAN_CAN_CLOCK_H
#define CAN_CAN_CLOCK_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "canconf.h"

/** measured in ticks, macros CAN_MS2TICK and CAN_TICK2MS should be used*/
typedef unsigned long can_clock_t;

/** Should be implemented by platform module, returns time of monotonic 32-bit clock*/
can_clock_t get_can_clock();

#define CAN_MS2TICK(t) ((t) / CAN_MS_PER_TICK)
#define CAN_TICK2MS(t) ((t) * CAN_MS_PER_TICK)

/** returns 1 if can timer timed out, 0 otherwise*/
static inline int can_timed_out(can_clock_t to) {
	return (to - get_can_clock()) >= 0x80000000 ? 1 : 0;
}

/**
 * returns
 * -1 if l < r,
 *  0 if l == r,
 *  1 if l > r
 */
static inline int can_time_compare(can_clock_t l, can_clock_t r)
{
	if ((l - r) >= 0x80000000)
		return -1;
	if (l == r)
		return 0;
	return 1;
}

/** sets timeout ahead of current time in ms*/
#define can_set_timeout(timer, timeout) do { timer = get_can_clock() + (CAN_MS2TICK(timeout)); } while (0)


#ifdef __cplusplus
}
#endif

#endif /*CAN_CAN_CLOCK_H*/
