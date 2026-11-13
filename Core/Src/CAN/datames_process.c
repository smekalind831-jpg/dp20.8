#include "CAN/datames_process.h"


//------------------------------------------------------------------------------
static const struct can_rx_slot can_j1939_datames_rx_slots[] = {
	{ .match_mask = CAN_MATCH_PDUFS_MASK, .match_id = 0xfed900, .port = CAN_MATCH_ANY_PORT }, //- канал AUXIO1
	{ .match_mask = CAN_MATCH_PDUF_MASK,  .match_id = 0xA70000, .port = CAN_MATCH_ANY_PORT }, //- канал AUXIO2
};

MAKE_IDLIST_FROM_RX_SLOT_ARRAY(can_j1939_datames_rx_id_list, can_j1939_datames_rx_slots);


//------------------------------------------------------------------------------
//- инициализация канала
//------------------------------------------------------------------------------
void j1939_datames_init()
{
	can_load_rx_id_list(CAN_DATAMES_PROCESS_CHANNEL, &can_j1939_datames_rx_id_list);
}


//AUXIO1_struct AUXIO1;
//AUXIO2_struct AUXIO2;

//------------------------------------------------------------------------------
//- обработка сообщений
//------------------------------------------------------------------------------
void datames_process(void)
{
	struct can_rx_message_header *header;
//	struct can_rx_single_message *smsg;
//  uint8_t *statusAUXIO2 = 0;
	

	for (;;) {
		header = can_engine_get_channel_message(CAN_DATAMES_PROCESS_CHANNEL);
		if (!header)
			break;

		
         if (!header->multi_packet) {
	//		smsg = can_to_rx_smsg(header);
        

      //  Rec_auxio2(&AUXIO2, smsg, 0xFF, AUXIO2_PGN, statusAUXIO2);
  /*    
      if(AUXIO2.auxio17 == 0b01)     //- управление первым выходом на датчике
      {
  //      out1_state = 1;     OUT_1_on;
      }
      else if(AUXIO2.auxio17 == 0b00 || AUXIO2.auxio17 == 0b10)
      {
  //      out1_state = 0;     OUT_1_off;
      }

      if(AUXIO2.auxio18 == 1)        //- управление вторым выходом на датчике
      {
  //      out2_state = 1;     OUT_2_on;
      }
      else if(AUXIO2.auxio18 == 0b00 || AUXIO2.auxio18 == 0b10)
      {
  //      out2_state = 0;     OUT_2_off;
      }
      */
      
      exchangeCount = 0;     //- сброс счетчика периодов без обмена по CAN (для включения габаритных огней на стреле)

//			if((smsg->frame.id & CAN_MATCH_PDUFS_MASK)==0xf00400)
//			{
//				//engine controller
//				if(*(uint16_t*)&smsg->frame.data[3] != 0xFFFF)
//				{
//					engine_data.speed=(*(uint16_t*)&smsg->frame.data[3])/8;
//					can_set_timeout(engine_data.timeout, 1000);
//				}
//			}			
		}else 
		{
			//mmsg = can_to_rx_mmsg(header);			
		}		
		can_engine_release_channel_message(CAN_DATAMES_PROCESS_CHANNEL);
	}  

}

