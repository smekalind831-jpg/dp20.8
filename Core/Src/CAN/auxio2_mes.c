
#ifndef AUXIO2TIMEOUT
#define AUXIO2TIMEOUT	2500
#endif


#include "CAN/auxio2_mes.h"
#include "CAN/idlist.h"

static void Set_default_data(AUXIO2_struct *pauxio2)
{
	pauxio2->data[0]=0xff;
	pauxio2->data[1]=0xff;
	pauxio2->data[2]=0xff;
	pauxio2->data[3]=0xff;
	pauxio2->data[4]=0xff;
	pauxio2->data[5]=0xff;
	pauxio2->data[6]=0xff;
	pauxio2->data[7]=0xff;
}

char Rec_auxio2(AUXIO2_struct *pauxio2, struct can_rx_single_message *smsg, uint8_t address, uint16_t pgn, uint8_t *status)
{

	char ret=0;
	unsigned char mask;
	if(address==255) mask=0;
	else mask=0xff;
	 if((smsg->frame.id & (CAN_MATCH_PDUF_MASK | mask))==((pgn<<8)|(address&mask)))
	 {
		 	pauxio2->data[0]=smsg->frame.data[0];
			pauxio2->data[1]=smsg->frame.data[1];
			pauxio2->data[2]=smsg->frame.data[2];
			pauxio2->data[3]=smsg->frame.data[3];
			pauxio2->data[4]=smsg->frame.data[4];
			pauxio2->data[5]=smsg->frame.data[5];
			pauxio2->data[6]=smsg->frame.data[6];
			pauxio2->data[7]=smsg->frame.data[7];
			can_set_timeout(pauxio2->fault_tmr, AUXIO2TIMEOUT);
			*status=0;
	 }
	 if(can_timed_out(pauxio2->fault_tmr))
	 {
		Set_default_data(pauxio2);
		*status=1;
	 }
	 return ret;
}

void auxio2_timeout(AUXIO2_struct *pauxio2, uint8_t *status)
{
	if(can_timed_out(pauxio2->fault_tmr))
	{
		Set_default_data(pauxio2);
		*status=1;
	}
}