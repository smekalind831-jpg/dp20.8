#ifndef CAN_TXDATA_BASE_H
#define CAN_TXDATA_BASE_H

#include "types.h"
#include "canconf.h"
#include "can_clock.h"

#define BROADCAST_RATE_INFINITE (~((can_clock_t)0))

struct txdata_base_parameter_description
{
	/*TODO:implement big/little endian support - host endian supported for now*/
	uint8_t start_byte;
	uint8_t start_bit;
	uint16_t bit_length;
	void *pdata;
};


unsigned txdata_base_build_data(const struct txdata_base_parameter_description *params, unsigned parameter_count, uint8_t *data, unsigned data_size);

#endif /*CAN_TXDATA_BASE_H*/
