#ifndef __BSP_BMA456_H__
#define __BSP_BMA456_H__

#include <stdint.h>
#include "apm32f10x_spi.h"

#include "main.h"
#include "board.h"

//- адреса регистров BMA456
#define PWR_CTRL        0x7D    //- 0xFD - вкл/выкл датчиков
#define PWR_CONF        0x7C    //- 0xFC - выбор режима питания
#define INTERNAL_ERROR  0x5F
#define FEATURES_IN     0x5E
#define INIT_CTRL       0x59    //- 0xD9 - start initialization
#define ACC_RANGE       0x41    //- 0xC1 - диапазон ускорений акселерометра
#define ACC_CONF        0x40    //- 0xC0 - конфигурация акселерометра
#define INTERNAL_STATUS 0x2A    //- 0xAA - error bits and message indicating internal status
#define TEMPERATURE     0x22    //- 0xA2 - датчик температуры: 0 -> 23 С, 0x80 - неверное значение
#define INT_STATUS_0    0x1C    //- 0x9C
#define SENSORTIME_0    0x18
#define ACC_Z_MSB       0x17    //- DATA13 - ускорение по оси Z
#define ACC_Z_LSB       0x16    //- DATA12
#define ACC_Y_MSB       0x15    //- DATA11 - ускорение по оси Y
#define ACC_Y_LSB       0x14    //- DATA10
#define ACC_X_MSB       0x13    //- DATA9 - ускорение по оси X
#define ACC_X_LSB       0x12    //- DATA8
#define STATUS          0x03    //- 0x83 - регистр статуса
#define ERR_REG         0x02    //- 0x82
#define CHIP_ID         0x00

//- коды ошибок
#define ERR_BMA456_ChipID       0x01    //- ошибка чтения идентификатора чипа BMA456
#define ERR_BMA456_ConfigFile   0x02    //- ошибка инициализации записи конфигурационного файла
#define ERR_BMA456_NotInit      0x04    //- чип не инициализирован
#define ERR_BMA456_ErrInit      0x08    //- ошибка инициализации
#define ERR_BMA456_ErrDriver    0x10    //- ошибка в файле конфигурации
#define ERR_BMA456_SnsStop      0x20    //- сенсор остановлен
#define ERR_BMA456_ReadReg      0x40    //- ошибка таймаута чтения регистра

void BMA456_init(void);
uint8_t BMA456_ReadReg(uint8_t reg);
void BMA456_Read_LSB_MSB(uint8_t reg, uint8_t *lsb, uint8_t *msb);
void BMA456_WriteReg(uint8_t reg, uint8_t byte);



#endif    //- __BSP_BMA456_H__