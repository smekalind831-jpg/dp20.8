#include "CAN/memrequest_process.h"
#include <string.h>
#include "CAN/idlist.h"
#include "CAN/warning.h"
#include "CAN/j1939_address_claim.h"
#include "CAN/txrx.h"
#include "CAN/calc_tsk.h"
#include "CAN/stm32f10x_bkp.h"
#include "CAN/stm32f10x_pwr.h"
#include "CAN/tn.h"

j1939_memaccess_data_struct j1939_memaccess_data;
//канал обработки запросов доступа к памяти
static const struct can_rx_slot can_j1939_memaccess_rx_slots[] = {
	{ .match_mask = CAN_MATCH_PDUF_MASK, .match_id = 0xd90000, .port = CAN_MATCH_ANY_PORT }, //request memory access
	{ .match_mask = CAN_MATCH_PDUF_MASK, .match_id = 0xd70000, .port = CAN_MATCH_ANY_PORT }, //binary data
};

static const struct can_rx_id_list can_j1939_memaccess_rx_id_list = {
	.data = can_j1939_memaccess_rx_slots,
	.size = 2,
};
extern PARAM RAMparameters[PARAMSNUM];
extern TN_SEM calc_sem;

uint8_t find_param(uint32_t param,uint8_t *index);

/**
 * \brief Module init function. Call at start.
 */
void j1939_memaccess_init()
{
	uint8_t tmp1,tmp2;
	for(tmp1=0;tmp1<CAN_INTERFACE_COUNT;tmp1++)
		for(tmp2=0;tmp2<CAN_MAX_SOURCE_ADDRESS_COUNT;tmp2++)
		{
			j1939_memaccess_data.states[tmp1][tmp2]=MEMACCESS_INITIAL;
		}
	//memset(&mdata, 0, sizeof(mdata));
	//memset(mdata.can_source_addresses, 0xfe, sizeof(mdata.can_source_addresses));
	j1939_memaccess_data.fl_access_enable[0][0]=1;
	
	can_load_rx_id_list(CAN_MEMORY_ACCESS_PROCESS_CHANNEL, &can_j1939_memaccess_rx_id_list);
}

uint8_t membuffer[userdatalen];
uint8_t tmpdata[8];
void memaccess_process(void)
{
	struct can_rx_message_header *header;
	struct can_tx_frame f;
	uint8_t cnt1,cnt2,tmp,tmp2,tmp3,bytes;
	uint32_t u32tmp1;
	
	struct can_rx_single_message *smsg;
	
	while(1)
	{
	
		for(cnt1=0;cnt1<CAN_INTERFACE_COUNT;cnt1++)
			for(cnt2=0;cnt2<CAN_MAX_SOURCE_ADDRESS_COUNT;cnt2++)
			{
				switch (j1939_memaccess_data.states[cnt1][cnt2])
				{
				case MEMACCESS_INITIAL:
					//проверяем состояние объявления адреса
					
					if(can_j1939_source_address(cnt1, cnt2)==0xfe) break;
					else j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_WORK;
					
					break;
				case MEMACCESS_WORK:
					//ожидаем запроса
					header = can_engine_get_channel_message(CAN_MEMORY_ACCESS_PROCESS_CHANNEL);
					if (!header)
						break;
					if (!header->multi_packet) 
					{
						smsg = can_to_rx_smsg(header);
						if(can_j1939_source_address(cnt1, cnt2)==0xfe) 
							j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_INITIAL;
						else
						{
							if((smsg->frame.id & CAN_MATCH_PDUF_MASK)==0xd90000)
							{
								if(j1939_memaccess_data.flmemaccess==0)
								{
									//получили запрос, и другого запроса в обработке нет
									if(((smsg->frame.id>>8)&CAN_MATCH_ANY_PORT)==can_j1939_source_address(cnt1, cnt2)&&
										 smsg->frame.dlc==8)
									{
										//запрос для нас
										//команда Bootload?
										if(smsg->frame.data[0]==0&&(smsg->frame.data[1]&0x1e)==0x0c&&
											 smsg->frame.data[2]==0x00&&smsg->frame.data[3]==0x00&&
												 smsg->frame.data[4]==0x00&&smsg->frame.data[5]==0x00&&
														 smsg->frame.data[6]==0xff&&smsg->frame.data[7]==0xff)
										{
											//приняли корректный запрос на вход в режим bootload
											j1939_memaccess_data.flmemaccess=1;
											j1939_memaccess_data.can_source_addresses[cnt1][cnt2]=(smsg->frame.id)&CAN_MATCH_ANY_PORT;
											j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_BOOTLOAD_ACCESS;
										}else	if(smsg->frame.data[0]<=PARAMSNUM&&(smsg->frame.data[1]&0x1e)==0x14&&
														smsg->frame.data[6]==0xff&&smsg->frame.data[7]==0xff)
										{
											//запрос на запись параметра
											
											j1939_memaccess_data.can_source_addresses[cnt1][cnt2]=(smsg->frame.id)&CAN_MATCH_ANY_PORT;
											bytes=0;
											for(u32tmp1=*(uint32_t*)&(smsg->frame.data[2]);u32tmp1<smsg->frame.data[0]+
																						*(uint32_t*)&(smsg->frame.data[2]);u32tmp1++)
												if(find_param(u32tmp1,&tmp3)==0)
												{
													bytes=0;
													break;
												}else bytes+=find_param(u32tmp1,&tmp3);
											if(bytes==0)
											{
												//из запрошенных параметров не все существую, отказываем в доступе
												f.id = can_compose_eid(6, 0xd8, j1939_memaccess_data.can_source_addresses[cnt1][cnt2], CAN_SOURCE_ADDRESS(cnt1, cnt2));
												f.dlc = 8;
												f.data[0] = 0;
												f.data[1] = 0x01 |1<<1;
												f.data[2] = 0xff;
												f.data[3] = 0xff;
												f.data[4] = 0xff;
												f.data[5] = 0xff;
												f.data[6] = 0xff;
												f.data[7] = 0xff;
												can_buffer_tx_data(&f, 0);
												j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_INITIAL;
											}else if(bytes>7)
											{
												//временно не поддерживаем передачу транспортным протоколом, отказываем в доступе
												f.id = can_compose_eid(6, 0xd8, j1939_memaccess_data.can_source_addresses[cnt1][cnt2], CAN_SOURCE_ADDRESS(cnt1, cnt2));
												f.dlc = 8;
												f.data[0] = 0;
												f.data[1] = 0x01 |1<<1;
												f.data[2] = 0xff;
												f.data[3] = 0xff;
												f.data[4] = 0xff;
												f.data[5] = 0xff;
												f.data[6] = 0xff;
												f.data[7] = 0xff;
												can_buffer_tx_data(&f, 0);
												j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_INITIAL;
											}else
											{
												j1939_memaccess_data.flmemaccess=1;
												j1939_memaccess_data.start_SPN=*(uint32_t*)&(smsg->frame.data[2]);
												j1939_memaccess_data.numSPNs=smsg->frame.data[0]+((uint16_t)(smsg->frame.data[1])&0xe0)*8;
												j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_WRITE0;
											}
										}else if(smsg->frame.data[0]<=PARAMSNUM&&(smsg->frame.data[1]&0x1e)==0x12&&
													 smsg->frame.data[6]==0xff&&smsg->frame.data[7]==0xff)
										{
											//запрос на чтение параметров
											j1939_memaccess_data.can_source_addresses[cnt1][cnt2]=(smsg->frame.id)&CAN_MATCH_ANY_PORT;
											bytes=0;
											for(u32tmp1=*(uint32_t*)&(smsg->frame.data[2]);u32tmp1<smsg->frame.data[0]+
																						*(uint32_t*)&(smsg->frame.data[2]);u32tmp1++)
												if(find_param(u32tmp1,&tmp3)==0)
												{
													bytes=0;
													break;
												}else bytes+=find_param(u32tmp1,&tmp3);
											if(bytes==0)
											{
												//из запрошенных параметров не все существую, отказываем в доступе
												f.id = can_compose_eid(6, 0xd8, j1939_memaccess_data.can_source_addresses[cnt1][cnt2], CAN_SOURCE_ADDRESS(cnt1, cnt2));
												f.dlc = 8;
												f.data[0] = 0;
												f.data[1] = 0x01 |1<<1;
												f.data[2] = 0xff;
												f.data[3] = 0xff;
												f.data[4] = 0xff;
												f.data[5] = 0xff;
												f.data[6] = 0xff;
												f.data[7] = 0xff;
												can_buffer_tx_data(&f, 0);
												j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_INITIAL;
											}else if(bytes>7)
											{
												//временно не поддерживаем передачу транспортным протоколом, отказываем в доступе
												f.id = can_compose_eid(6, 0xd8, j1939_memaccess_data.can_source_addresses[cnt1][cnt2], CAN_SOURCE_ADDRESS(cnt1, cnt2));
												f.dlc = 8;
												f.data[0] = 0;
												f.data[1] = 0x01 |1<<1;
												f.data[2] = 0xff;
												f.data[3] = 0xff;
												f.data[4] = 0xff;
												f.data[5] = 0xff;
												f.data[6] = 0xff;
												f.data[7] = 0xff;
												can_buffer_tx_data(&f, 0);
												j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_INITIAL;
											}else
											{
												j1939_memaccess_data.flmemaccess=1;
												j1939_memaccess_data.start_SPN=*((uint32_t*)&(smsg->frame.data[2]));
												j1939_memaccess_data.numSPNs=smsg->frame.data[0]+((uint16_t)(smsg->frame.data[1])&0xe0)*8;
												j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_READ0;
											}
										}
									}
								}else
								{
									//уже заняты другим процессом
									f.id = can_compose_eid(6, 0xd8, j1939_memaccess_data.can_source_addresses[cnt1][cnt2], CAN_SOURCE_ADDRESS(cnt1, cnt2));
									f.dlc = 8;
									f.data[0] = 0;
									f.data[1] = 0x01 |1<<1;
									f.data[2] = 0xff;
									f.data[3] = 0xff;
									f.data[4] = 0xff;
									f.data[5] = 0xff;
									f.data[6] = 0xff;
									f.data[7] = 0xff;
									can_buffer_tx_data(&f, 0);
									j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_INITIAL;
								}
							}
						}
					}
					break;
				case MEMACCESS_BOOTLOAD_ACCESS:
					//отправляем подтверждение приема запроса
					f.id = can_compose_eid(6, 0xd8, j1939_memaccess_data.can_source_addresses[cnt1][cnt2], CAN_SOURCE_ADDRESS(cnt1, cnt2));
					f.dlc = 8;
					f.data[0] = 0;
					f.data[1] = 0x01;
					f.data[2] = 0xff;
					f.data[3] = 0xff;
					f.data[4] = 0xff;
					f.data[5] = 0xff;
					f.data[6] = 0xff;
					f.data[7] = 0xff;
					can_buffer_tx_data(&f, 0);
					//отправляем подтверждение окончания сеанса
					f.id = can_compose_eid(6, 0xd8, j1939_memaccess_data.can_source_addresses[cnt1][cnt2], CAN_SOURCE_ADDRESS(cnt1, cnt2));
					f.dlc = 8;
					f.data[0] = 0;
					f.data[1] = 0x01|4<<1;
					f.data[2] = 0xff;
					f.data[3] = 0xff;
					f.data[4] = 0xff;
					f.data[5] = 0xff;
					f.data[6] = 0xff;
					f.data[7] = 0xff;
					can_buffer_tx_data(&f, 0);
					can_set_timeout(j1939_memaccess_data.memory_access_timeout_time[cnt1][cnt2], 250);
					j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_BOOTLOAD1;
					break;
				case MEMACCESS_BOOTLOAD1:
					//ожидаем прихода подтверждения закрытия сессии
					//проверка таймаута
					if(can_timed_out(j1939_memaccess_data.memory_access_timeout_time[cnt1][cnt2]))
					{
						//превышен таймаут, сбрасываем сессию
						j1939_memaccess_data.flmemaccess=0;
						j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_INITIAL;
					}
					header = can_engine_get_channel_message(CAN_MEMORY_ACCESS_PROCESS_CHANNEL);
					if (!header)
						break;
					if (!header->multi_packet) 
					{
						smsg = can_to_rx_smsg(header);
						if(can_j1939_source_address(cnt1, cnt2)==0xfe) 
							j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_INITIAL;
						else
						{
							if((smsg->frame.id & CAN_MATCH_PDUF_MASK)==0xd90000&&
								 ((smsg->frame.id>>8)&CAN_MATCH_ANY_PORT)==can_j1939_source_address(cnt1, cnt2)&&
									 ((smsg->frame.id)&CAN_MATCH_ANY_PORT)==j1939_memaccess_data.can_source_addresses[cnt1][cnt2]&&
									 smsg->frame.dlc==8&& smsg->frame.data[0]==0&&smsg->frame.data[1]==0x09)
							{
								//приняли подтверждение перехода в загрузчик
								PWR_BackupAccessCmd(ENABLE);
								BKP_WriteBackupRegister(BKP_DR1, 0xaa55);
								BKP_WriteBackupRegister(BKP_DR2, j1939_memaccess_data.can_source_addresses[cnt1][cnt2]);
								NVIC_SystemReset();
							}
						}
					}
					break;
				case MEMACCESS_READ0:
					//подтверждаем сессию
					f.id = can_compose_eid(6, 0xd8, j1939_memaccess_data.can_source_addresses[cnt1][cnt2], CAN_SOURCE_ADDRESS(cnt1, cnt2));
					f.dlc = 8;
					f.data[0] = j1939_memaccess_data.numSPNs&0xff;
					f.data[1] = 0x01|(j1939_memaccess_data.numSPNs&0x700)>>3;
					f.data[2] = 0xff;
					f.data[3] = 0xff;
					f.data[4] = 0xff;
					f.data[5] = 0xff;
					f.data[6] = 0xff;
					f.data[7] = 0xff;
					if(can_buffer_tx_data(&f, 0)!=0) j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_READ1;
					
					
					break;
				case MEMACCESS_READ1:
					//подготавливаем  данные
					bytes=1;
					memset(f.data,0xff,8);
					for(tmp=0;tmp<j1939_memaccess_data.numSPNs;tmp++)
					{
						tmp2=find_param(j1939_memaccess_data.start_SPN+tmp,&tmp3);
						if(tmp2==1) 
						{
							f.data[bytes]=*(uint32_t*)(RAMparameters[tmp3].pvalue);
							bytes++;
						}
						else if(tmp2==2)
						{
							*(uint16_t*)&f.data[bytes]=*(uint32_t*)(RAMparameters[tmp3].pvalue);
							bytes+=2;
						}else if(tmp2==4)
						{
							*(uint32_t*)&f.data[bytes]=*(uint32_t*)(RAMparameters[tmp3].pvalue);
							bytes+=4;
						}else if(tmp2==8)
						{
//							*(uint64_t*)&f.data[bytes]=*(uint32_t*)(RAMparameters[tmp3].pvalue);
//							bytes+=8;
						}
					}
					f.id = can_compose_eid(6, 0xd7, j1939_memaccess_data.can_source_addresses[cnt1][cnt2], CAN_SOURCE_ADDRESS(cnt1, cnt2));
					f.data[0]=bytes-1;
					f.dlc = 8;
					//отправляем данные
					if(can_buffer_tx_data(&f, 0)!=0) j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_READ3;
					break;
				case MEMACCESS_READ3:
					//отправляем подтверждение окончания сессии
					f.id = can_compose_eid(6, 0xd8, j1939_memaccess_data.can_source_addresses[cnt1][cnt2], CAN_SOURCE_ADDRESS(cnt1, cnt2));
					f.dlc = 8;
					f.data[0] = 0;
					f.data[1] = 0x01|4<<1;
					f.data[2] = 0xff;
					f.data[3] = 0xff;
					f.data[4] = 0xff;
					f.data[5] = 0xff;
					f.data[6] = 0xff;
					f.data[7] = 0xff;
					if(can_buffer_tx_data(&f, 0)!=0) j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_READ4;
					break;
				case MEMACCESS_READ4:
					
					can_set_timeout(j1939_memaccess_data.memory_access_timeout_time[cnt1][cnt2], 250);
					j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_READ5;
					break;
				case MEMACCESS_READ5:
					//ожидаем подтверждения закрытия сессии
					if(can_timed_out(j1939_memaccess_data.memory_access_timeout_time[cnt1][cnt2]))
					{
						//timeout
						j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_INITIAL;
						j1939_memaccess_data.flmemaccess=0;
						break;
					}
					header = can_engine_get_channel_message(CAN_MEMORY_ACCESS_PROCESS_CHANNEL);
					if (!header)
						break;
					if (!header->multi_packet) 
					{
						smsg = can_to_rx_smsg(header);
						if(((smsg->frame.id)&0xff00ff)==0xd90000|j1939_memaccess_data.can_source_addresses[cnt1][cnt2])
						{
							if(((smsg->frame.data[1])&0x1e)==0x18||((smsg->frame.data[1])&0x1e)==0x14||
								 ((smsg->frame.data[1])&0x1e)==0x12)
							{
								if(((smsg->frame.data[1])&0x1e)==0x18) 
								{
									j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_INITIAL;
									j1939_memaccess_data.flmemaccess=0;
								}
								else if(((smsg->frame.data[1])&0x1e)==0x14) 
								{
									j1939_memaccess_data.flmemaccess=1;
									j1939_memaccess_data.start_SPN=*(uint32_t*)&(smsg->frame.data[2]);
									j1939_memaccess_data.numSPNs=smsg->frame.data[0]+((uint16_t)(smsg->frame.data[1])&0xe0)*8;
									j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_WRITE0;
								}
								else 
								{
									j1939_memaccess_data.flmemaccess=1;
									j1939_memaccess_data.start_SPN=*(uint32_t*)&(smsg->frame.data[2]);
									j1939_memaccess_data.numSPNs=smsg->frame.data[0]+((uint16_t)(smsg->frame.data[1])&0xe0)*8;
									j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_READ0;
								}
							}else 
							{
								j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_INITIAL;
								j1939_memaccess_data.flmemaccess=0;
							}
						}
					}
					break;
				case MEMACCESS_WRITE0:
					//подтверждаем сессию
					f.id = can_compose_eid(6, 0xd8, j1939_memaccess_data.can_source_addresses[cnt1][cnt2], CAN_SOURCE_ADDRESS(cnt1, cnt2));
					f.dlc = 8;
					f.data[0] = j1939_memaccess_data.numSPNs&0xff;
					f.data[1] = 0x01|(j1939_memaccess_data.numSPNs&0x700)>>3;
					f.data[2] = 0xff;
					f.data[3] = 0xff;
					f.data[4] = 0xff;
					f.data[5] = 0xff;
					f.data[6] = 0xff;
					f.data[7] = 0xff;
					if(can_buffer_tx_data(&f, 0)!=0) j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_WRITE1;
					can_set_timeout(j1939_memaccess_data.memory_access_timeout_time[cnt1][cnt2], 250);
					
					break;
				case MEMACCESS_WRITE1:
					//ожидаем данные
					if(can_timed_out(j1939_memaccess_data.memory_access_timeout_time[cnt1][cnt2]))
					{
						//timeout
						j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_INITIAL;
						j1939_memaccess_data.flmemaccess=0;
						break;
					}
					header = can_engine_get_channel_message(CAN_MEMORY_ACCESS_PROCESS_CHANNEL);
					if (!header)
						break;
					if (!header->multi_packet) 
					{
						smsg = can_to_rx_smsg(header);
						if(((smsg->frame.id)&0xff00ff)==0xd70000|j1939_memaccess_data.can_source_addresses[cnt1][cnt2])
						{
							//приняли данные
							for(tmp=0;tmp<8;tmp++) tmpdata[tmp]=smsg->frame.data[tmp];
							j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_WRITE2;
							
						}
					}
					break;
				case MEMACCESS_WRITE2:
					//отправляем подтверждение окончания сессии
					f.id = can_compose_eid(6, 0xd8, j1939_memaccess_data.can_source_addresses[cnt1][cnt2], CAN_SOURCE_ADDRESS(cnt1, cnt2));
					f.dlc = 8;
					f.data[0] = 0;
					f.data[1] = 0x01|4<<1;
					f.data[2] = 0xff;
					f.data[3] = 0xff;
					f.data[4] = 0xff;
					f.data[5] = 0xff;
					f.data[6] = 0xff;
					f.data[7] = 0xff;
					if(can_buffer_tx_data(&f, 0)!=0) j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_WRITE3;
					can_set_timeout(j1939_memaccess_data.memory_access_timeout_time[cnt1][cnt2], 250);
					break;
				case MEMACCESS_WRITE3:
					//ожидаем подтверждения окончания сессии
					if(can_timed_out(j1939_memaccess_data.memory_access_timeout_time[cnt1][cnt2]))
					{
						//timeout
						j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_INITIAL;
						j1939_memaccess_data.flmemaccess=0;
						break;
					}
					header = can_engine_get_channel_message(CAN_MEMORY_ACCESS_PROCESS_CHANNEL);
					if (!header)
						break;
					if (!header->multi_packet) 
					{
						smsg = can_to_rx_smsg(header);
						if(((smsg->frame.id)&0xff00ff)==0xd90000|j1939_memaccess_data.can_source_addresses[cnt1][cnt2])
						{
							if(((smsg->frame.data[1])&0x1e)==0x18||((smsg->frame.data[1])&0x1e)==0x14||
								 ((smsg->frame.data[1])&0x1e)==0x12)
							{
								if(((smsg->frame.data[1])&0x1e)==0x18) 
								{
									j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_WRITE4;
									j1939_memaccess_data.flmemaccess=0;
								}
								else if(((smsg->frame.data[1])&0x1e)==0x14) 
								{
//									j1939_memaccess_data.flmemaccess=1;
//									j1939_memaccess_data.start_SPN=*(uint32_t*)&(smsg->frame.data[2]);
//									j1939_memaccess_data.numSPNs=smsg->frame.data[0]+((uint16_t)(smsg->frame.data[1])&0xe0)*8;
									j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_WRITE5;
								}
								else 
								{
//									j1939_memaccess_data.flmemaccess=1;
//									j1939_memaccess_data.start_SPN=*(uint32_t*)&(smsg->frame.data[2]);
//									j1939_memaccess_data.numSPNs=smsg->frame.data[0]+((uint16_t)(smsg->frame.data[1])&0xe0)*8;
									j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_WRITE6;
								}
							}else 
							{
								j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_INITIAL;
								j1939_memaccess_data.flmemaccess=0;
								break;
							}
						}
					}

				case MEMACCESS_WRITE4:
				case MEMACCESS_WRITE5:
				case MEMACCESS_WRITE6:
					//перезаписываем принятые параметры
					tmp2=0;
					for(tmp=0;tmp<PARAMSNUM;tmp++)
						if(RAMparameters[tmp].SPN==j1939_memaccess_data.start_SPN) break;
//					for(;tmp<j1939_memaccess_data.numSPNs;tmp++)
//					{
						if((uint32_t)RAMparameters[tmp].pvalue>=ApplicationAddress&&
							 (uint32_t)RAMparameters[tmp].pvalue<=ApplicationAddress+userdatalen-4)
						{
							//параметр во FLASH
							tn_mutex_lock(&FLASH_mutex, TN_WAIT_INFINITE);
							memcpy(membuffer,(uint8_t*)ApplicationAddress,userdatalen);
							for(u32tmp1=0;u32tmp1<tmpdata[0];u32tmp1++)
							{
								membuffer[(uint32_t)RAMparameters[tmp].pvalue-ApplicationAddress+u32tmp1]=tmpdata[u32tmp1+1];
							}
							
							UserDataupdate(membuffer,userdatalen-4);
							tn_mutex_unlock(&FLASH_mutex);
						}else if((uint32_t)RAMparameters[tmp].pvalue>=RAMstartaddr&&
							 (uint32_t)RAMparameters[tmp].pvalue<=RAMstartaddr+RAMsize)
						{
							//параметр в ОЗУ
							if(RAMparameters[tmp].size==1)
							{
								
								*(uint8_t*)(RAMparameters[tmp].pvalue)=tmpdata[tmp2];
								tmp2++;
								can_set_timeout(RAMparameters[tmp].timeout, RAMparameters[tmp].deftimeout);
								tn_sem_signal (&calc_sem);
							}else if(RAMparameters[tmp].size==2)
							{
								*(uint16_t*)(RAMparameters[tmp].pvalue)=*(uint16_t*)&tmpdata[tmp2];
								tmp2+=2;
								can_set_timeout(RAMparameters[tmp].timeout, RAMparameters[tmp].deftimeout);
								tn_sem_signal (&calc_sem);
							}else if(RAMparameters[tmp].size==4)
							{
								*(uint32_t*)(RAMparameters[tmp].pvalue)=*(uint32_t*)&tmpdata[tmp2];
								tmp2+=4;
								can_set_timeout(RAMparameters[tmp].timeout, RAMparameters[tmp].deftimeout);
								tn_sem_signal (&calc_sem);
							}else
							{
								*(uint64_t*)RAMparameters[tmp].pvalue=*(uint64_t*)&tmpdata[tmp2];
								tmp2+=8;
								can_set_timeout(RAMparameters[tmp].timeout, RAMparameters[tmp].deftimeout);
								tn_sem_signal (&calc_sem);
							}
						}
//					}
					if(j1939_memaccess_data.states[cnt1][cnt2]==MEMACCESS_WRITE4) 
					{
						j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_INITIAL;
						j1939_memaccess_data.flmemaccess=0;
					}
					else if(j1939_memaccess_data.states[cnt1][cnt2]==MEMACCESS_WRITE5) 
					{
						j1939_memaccess_data.flmemaccess=1;
						j1939_memaccess_data.start_SPN=*(uint32_t*)&(smsg->frame.data[2]);
						j1939_memaccess_data.numSPNs=smsg->frame.data[0]+((uint16_t)(smsg->frame.data[1])&0xe0)*8;
						j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_WRITE0;
					}
					else 
					{
						j1939_memaccess_data.flmemaccess=1;
						j1939_memaccess_data.start_SPN=*(uint32_t*)&(smsg->frame.data[2]);
						j1939_memaccess_data.numSPNs=smsg->frame.data[0]+((uint16_t)(smsg->frame.data[1])&0xe0)*8;
						j1939_memaccess_data.states[cnt1][cnt2]=MEMACCESS_READ0;
					}
					
				}
			}
		break;
	}
	can_engine_release_channel_message(CAN_MEMORY_ACCESS_PROCESS_CHANNEL);
}

//ф-я поиска параметра 
//возвращает размер параметра в байтах
//если параметр не найден - возвращает 0
uint8_t find_param(uint32_t param,uint8_t *index)
{
	uint8_t tmp;
	for(tmp=0;tmp<PARAMSNUM;tmp++)
		if(param==RAMparameters[tmp].SPN)
		{
			//у нас есть запрашиваемый параметр
			*index=tmp;
			return RAMparameters[tmp].size;
		}
	return 0;
}

