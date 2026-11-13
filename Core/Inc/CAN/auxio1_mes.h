

#ifndef AUXIO1_H
#define AUXIO1_H

#include "types.h"
#include "canengine.h"
#include "rec_mes.h"

#define AUXIO1_PGN	0xfed9


typedef struct
{
	union
	{
		CANMES_struct canmes;
		struct
		{
			uint8_t auxio4:2;
			uint8_t auxio3:2;
			uint8_t auxio2:2;
			uint8_t auxio1:2;
			uint8_t auxio8:2;
			uint8_t auxio7:2;
			uint8_t auxio6:2;
			uint8_t auxio5:2;
			uint8_t auxio12:2;
			uint8_t auxio11:2;
			uint8_t auxio10:2;
			uint8_t auxio9:2;
			uint8_t auxio16:2;
			uint8_t auxio15:2;
			uint8_t auxio14:2;
			uint8_t auxio13:2;
			uint16_t IOchannels[2];
		};
	};
	
}AUXIO1_struct;

#endif //AUXIO1