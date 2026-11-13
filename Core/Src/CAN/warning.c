#include "CAN/warning.h"

//#include "can_debug.h"

/**
 * \defgroup Warnings Warnings
 * \brief CAN Stack warnings set/reset/test routines
 * @{
 */

uint8_t can_warning_flags[CAN_MAX_MODULES];

/** Sets warning flag. Use is_can_warning_flag_set to extract value and can_reset_warning_flag to reset it*/
void can_set_warning_flag(uint8_t module, uint8_t flag)
{
	if (module >= CAN_MAX_MODULES)
		return;

#ifdef CAN_DEBUG
	if (!(can_warning_flags[module] & (1 << flag)))
		can_dbg("Setting warning %u:%u\n", module, flag);
#endif

	can_warning_flags[module] |= 1 << flag;
}

/** Resets warning flag */
void can_reset_warning_flag(uint8_t module, uint8_t flag)
{
	if (module >= CAN_MAX_MODULES)
		return;

	can_warning_flags[module] &= 1 << flag;
}

/** Returns non-zero value for set flag and 0 for non-set */
int is_can_warning_flag_set(uint8_t module, uint8_t flag)
{
	if (module >= CAN_MAX_MODULES)
		return 0;

	return can_warning_flags[module] & (1 << flag);
}

/** Returns non-zero value if any warning flags is set and assigns first set to (module,flag)*/
int can_warning_get_and_reset_next_flag(uint8_t *module, uint8_t *flag)
{
	unsigned i;
	unsigned j;

	for (i = 0; i < CAN_MAX_MODULES; ++i) {
		uint8_t flags = can_warning_flags[i];
		for (j = 0; j < 8; ++j) {
			if (flags & (1 << j)) {
				*module = i;
				*flag = j;
				can_warning_flags[i] &= ~(1 << j);
				return 1;
			}
		}
	}
	return 0;
}

/** @} */
