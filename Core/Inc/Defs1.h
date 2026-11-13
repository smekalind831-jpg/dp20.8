
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
//#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_spi.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h"
#include "About.h"   // дата версия
#include "stm32f1xx_ll_wwdg.h"  

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif

//\----- CAN -------------

//#include "CAN/apm32f10x_can.h"
//#include "stm32f1xx_hal_can.h"
/*
#include "CAN/prg_struct.h"
#include "CAN/peripheral_init.h" 
#include "CAN/fstore_params.h"
 */
#include "CAN/bsp_can.h"
/*#include "CAN/canengine.h"
#include "CAN/transport_protocol.h"
#include "CAN/txdata.h"
#include "CAN/j1939_address_claim.h"
//#include "datames_process.h"
 */
extern void j1939_datames_init(void);
extern void j1939_address_claim_init();
extern void j1939_memory_access_device_init(void);
extern void can_transport_protocol_init();
extern void can_transport_protocol_process();
extern void can_transport_protocol_check_timeout();
extern void can_engine_init();
extern void can_engine_process_rx_queue();
extern void can_txdata_init();
extern void can_txrx_init();
extern void can_tx_data();
extern void can_txrx_purge_rx_queue();
extern void j1939_address_claim_process();
extern void can_txdata_process();
extern void j1939_memory_access_device_process(void);
//extern uint8_t CAN_ReadFlagStatus(CAN_T* CANx, CAN_FLAG_T flag);
//extern void can_txdata_process_txmessage(unsigned index, uint8_t da, can_clock_t timeout);
//extern void can_txrx_purge_tx_queue(can_port_t port);
 
/* Порты ---------------------------------------------------------------------*/
#define CanEN_Pin LL_GPIO_PIN_1
#define CanEN_GPIO_Port GPIOA
//------  ОТЛАДКА ---------
//#define MS_Pin   CanEN_Pin    // MS LL_GPIO_PIN_10 PB10  //\ Отладка

//------  ОТЛАДКА END -----  
#define LIN_Pin LL_GPIO_PIN_2
#define LIN_GPIO_Port GPIOA
#define SCK1_Pin LL_GPIO_PIN_5
#define SCK1_GPIO_Port GPIOA
#define MISO1_Pin LL_GPIO_PIN_6
#define MISO1_GPIO_Port GPIOA
#define MOSI1_Pin LL_GPIO_PIN_7
#define MOSI1_GPIO_Port GPIOA
#define CS1_Pin LL_GPIO_PIN_0
#define CS1_GPIO_Port GPIOB
#define CS2_Pin LL_GPIO_PIN_1
#define CS2_GPIO_Port GPIOB
#define CS3_Pin LL_GPIO_PIN_2
#define CS3_GPIO_Port GPIOB
#define MS_Pin LL_GPIO_PIN_10 //0x0400 //     // MS PB10  //\ Отладка
#define HOLD_Pin LL_GPIO_PIN_11 //0x0800 //   // HOLD PB11  //\ Отладка
#define MS_Port GPIOB
#define CSx_Port GPIOB                   
#define SCK2_Pin LL_GPIO_PIN_13
#define SCK2_GPIO_Port GPIOB
#define MISO2_Pin LL_GPIO_PIN_14
#define MISO2_GPIO_Port GPIOB
#define MOSI2_Pin LL_GPIO_PIN_15
#define MOSI2_GPIO_Port GPIOB
#define U1_TX_Pin LL_GPIO_PIN_9
#define U1_TX_GPIO_Port GPIOA
#define U1_RX_Pin LL_GPIO_PIN_10
#define U1_RX_GPIO_Port GPIOA
#define Off_Port GPIOB
#define Off_In 0x1000 // Off
#define Off_EXTI_IRQn EXTI15_10_IRQn

