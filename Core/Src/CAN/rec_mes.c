


#include "CAN/rec_mes.h"
#include "CAN/idlist.h"

static void Set_default_data(CANMES_struct *pmes)
{
	pmes->data[0]=0xff;
	pmes->data[1]=0xff;
	pmes->data[2]=0xff;
	pmes->data[3]=0xff;
	pmes->data[4]=0xff;
	pmes->data[5]=0xff;
	pmes->data[6]=0xff;
	pmes->data[7]=0xff;
}

char Rec_canmes(CANMES_struct *pmes, struct can_rx_single_message *smsg, uint8_t address, uint16_t pgn, uint32_t timeout)
{

	char ret=0;
	 if((address!=0xff && (smsg->frame.id & (0xffffff))==((pgn<<8)|address)) ||
			(address==0xff && (smsg->frame.id & (0xffff00))==((pgn<<8))))
	 {
		 	pmes->data[0]=smsg->frame.data[0];
			pmes->data[1]=smsg->frame.data[1];
			pmes->data[2]=smsg->frame.data[2];
			pmes->data[3]=smsg->frame.data[3];
			pmes->data[4]=smsg->frame.data[4];
			pmes->data[5]=smsg->frame.data[5];
			pmes->data[6]=smsg->frame.data[6];
			pmes->data[7]=smsg->frame.data[7];
			can_set_timeout(pmes->fault_tmr, timeout);
			ret=1;
	 }
	 return ret;
}

char recmes_timeout(CANMES_struct *pmes)
{
	char ret=0;
	if(can_timed_out(pmes->fault_tmr))
	{
		Set_default_data(pmes);
		ret=1;
	}
	return ret;
}