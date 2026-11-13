#include "CAN/txdata_base.h"

#include <string.h>

#include "CAN/warning.h"

#define MASK_FROM_BIT_LENGTH(len) ((1 << len) - 1)

unsigned txdata_base_build_data(const struct txdata_base_parameter_description *params, unsigned parameter_count, uint8_t *data, unsigned data_size)
{
	unsigned i;
	unsigned msgsize = 0;

	for (i = 0; i<parameter_count; ++i) {
		/*TODO: prevent out of 8 bytes assignments*/
		const struct txdata_base_parameter_description *p = &params[i];
		if ((p->start_bit > 0) && (p->bit_length > 8 - p->start_bit)) {
			//not implemented
			can_set_warning_flag(CAN_TXDATA_MODULE, 7);
			continue;
		}
		uint8_t *pdata = (uint8_t*)(p->pdata);
		if (p->bit_length % 8) {
			data[p->start_byte] = (data[p->start_byte] & ~(MASK_FROM_BIT_LENGTH(p->bit_length) << p->start_bit))
				| (pdata[0] & MASK_FROM_BIT_LENGTH(p->bit_length)) << p->start_bit;
		} else {
			memcpy(data + p->start_byte, pdata, p->bit_length / 8);
		}
		unsigned newsize = p->start_byte + (p->bit_length + 7) / 8;
		if (msgsize < newsize)
			msgsize = newsize;
	}
	return msgsize;
}
