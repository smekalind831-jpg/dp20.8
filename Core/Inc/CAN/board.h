#ifndef __BOARD_H__
#define __BOARD_H__

#ifdef __cplusplus
 extern "C" {
#endif

//- адресация с использованием Bit Banding
#define PortA_bit10   (*((uint32_t *)0x422101A8))
#define PortA_bit15   (*((uint32_t *)0x422101BC))

#define PortB_bit0    (*((uint32_t *)0x42218180))
#define PortB_bit1    (*((uint32_t *)0x42218184))
#define PortB_bit2    (*((uint32_t *)0x42218188))
#define PortB_bit4    (*((uint32_t *)0x42218190))
#define PortB_bit5    (*((uint32_t *)0x42218194))
#define PortB_bit6    (*((uint32_t *)0x42218198))
#define PortB_bit7    (*((uint32_t *)0x4221819C))
#define PortB_bit9    (*((uint32_t *)0x422181A4))
#define PortB_bit11   (*((uint32_t *)0x422181AC))
#define PortB_bit15   (*((uint32_t *)0x422181BC))

#define CS_BMA456     PortB_bit0      //- chip select BMA456 для SPI-интерфейса
#define CS_TEMP       PortB_bit1      //- chip select датчика температуры для SPI-интерфейса
#define CAN_EN        PortB_bit4      //- выбор м/с CAN-драйвера: 0 - отключен, 1 - включен
#define AMP           PortB_bit5      //- управление усилителем фильтра: 0 - отключен, 1 - включен
#define OUT_1         PortB_bit6      //- силовой выход 1: 0 - отключен, 1 - включен
#define OUT_2         PortB_bit7      //- силовой выход 2: 0 - отключен, 1 - включен
#define LIN_EN        PortA_bit15     //- выбор м/с LIN-драйвера: 0 - отключен, 1 - включен


#define AMP_on        AMP = 1;        //- 1: включить усилитель фильтра
#define AMP_off       AMP = 0;        //- 0: отключить усилитель фильтра

#define CAN_on        CAN_EN = 0;     //- 0: включить CAN
#define CAN_off       CAN_EN = 1;     //- 1: выключить CAN

#define LIN_on        LIN_EN = 1;     //- 1: включить LIN (RBus)
#define LIN_off       LIN_EN = 0;     //- 0: выключить LIN (RBus)

#define CS_BMA456_on    CS_BMA456 = 0;    //- разрешить доступ по SPI к BMA456
#define CS_BMA456_off   CS_BMA456 = 1;    //- запретить доступ по SPI к BMA456

#define CS_TEMP_on    CS_TEMP = 0;    //- разрешить доступ по SPI к датчику температуры
#define CS_TEMP_off   CS_TEMP = 1;    //- запретить доступ по SPI к датчику температуры

#define OUT_1_on      OUT_1 = 1;      //- включить первый силовой выход
#define OUT_1_off     OUT_1 = 0;      //- выключить первый силовой выход
#define OUT_2_on      OUT_2 = 1;      //- включить второй силовой выход
#define OUT_2_off     OUT_2 = 0;      //- выключить второй силовой выход

#ifdef __cplusplus
}
#endif
  
#endif    //- __BOARD_H__
