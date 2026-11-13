#ifndef AUXIO2_H
#define AUXIO2_H

#include "types.h"
#include "canengine.h"

#define AUXIO2_PGN	0xa700


typedef struct
{
	union
	{
		uint8_t data[8];
		struct
		{
			uint8_t auxio20:2;
			uint8_t auxio19:2;
			uint8_t auxio18:2;
			uint8_t auxio17:2;
			uint8_t auxio24:2;
			uint8_t auxio23:2;
			uint8_t auxio22:2;
			uint8_t auxio21:2;
			uint8_t auxio28:2;
			uint8_t auxio27:2;
			uint8_t auxio26:2;
			uint8_t auxio25:2;
			uint8_t auxio32:2;
			uint8_t auxio31:2;
			uint8_t auxio30:2;
			uint8_t auxio29:2;
			uint8_t auxio36:2;
			uint8_t auxio35:2;
			uint8_t auxio34:2;
			uint8_t auxio33:2;
			uint8_t auxio40:2;
			uint8_t auxio39:2;
			uint8_t auxio38:2;
			uint8_t auxio37:2;
			uint8_t auxio44:2;
			uint8_t auxio43:2;
			uint8_t auxio42:2;
			uint8_t auxio41:2;
			uint8_t auxio48:2;
			uint8_t auxio47:2;
			uint8_t auxio46:2;
			uint8_t auxio45:2;
		};
	};
	uint32_t fault_tmr;
}AUXIO2_struct;


char Rec_auxio2(AUXIO2_struct *pauxio2, struct can_rx_single_message *smsg, uint8_t address, uint16_t pgn, uint8_t *status);
void auxio2_timeout(AUXIO2_struct *pauxio2, uint8_t *status);
#endif //AUXIO2