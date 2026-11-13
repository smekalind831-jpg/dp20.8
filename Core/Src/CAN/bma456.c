#include "bma456.h"

uint8_t lsb, msb;
int32_t sum_X, sum_Y, sum_Z;      //- усреднение показаний
int32_t arr_X[16], arr_Y[16], arr_Z[16];
uint8_t arr_count;    //- 
//uint16_t acc_X_out, acc_Y_out, acc_Z_out;      //- ускорения по осям с учетом знака

float deg_X, deg_Y, deg_Z;
int16_t ang_X, ang_Y, ang_Z;
uint8_t angleX, angleY, angleZ;

float cos_angle;    //- косинус угла между векторами наклона стрелы: калибровочным и фактическим


void AngleCalc()
{
  BMA456_ReadReg(PWR_CONF);
  BMA456_ReadReg(CHIP_ID);


  BMA456_ReadReg(ERR_REG);
  BMA456_ReadReg(ACC_CONF);
  BMA456_ReadReg(ACC_RANGE);

  BMA456_ReadReg(INTERNAL_ERROR);
  BMA456_ReadReg(INTERNAL_STATUS);

  
  
  //- углы
  if(BMA456_ReadReg(STATUS) & 0x80)
  {
//- X
    lsb = BMA456_ReadReg(ACC_X_LSB);
    msb = BMA456_ReadReg(ACC_X_MSB);
    acc_X = (int16_t)(lsb | ((uint16_t)msb)<<8);
//- Y
    lsb = BMA456_ReadReg(ACC_Y_LSB);
    msb = BMA456_ReadReg(ACC_Y_MSB);
    acc_Y = (int16_t)(lsb | ((uint16_t)msb)<<8);
//- Z  
    lsb = BMA456_ReadReg(ACC_Z_LSB);
    msb = BMA456_ReadReg(ACC_Z_MSB);
    acc_Z = (int16_t)(lsb | ((uint16_t)msb)<<8);

    //- усреднение показаний
    if(++arr_count > 15)   arr_count = 0;
    arr_X[arr_count] = acc_X;
    arr_Y[arr_count] = acc_Y;
    arr_Z[arr_count] = acc_Z;
    sum_X = sum_Y = sum_Z = 0.0;
    for(uint8_t i=0; i<16; i++)   { sum_X += arr_X[i];  sum_Y += arr_Y[i];  sum_Z += arr_Z[i]; }
    acc_X = sum_X/16;
    acc_Y = sum_Y/16;
    acc_Z = sum_Z/16;
//    acc_X_out = (uint16_t)(acc_X + 8192);
//    acc_Y_out = (uint16_t)(acc_Y + 8192);
//    acc_Z_out = (uint16_t)(acc_Z + 8192);
    acc_X_out = (uint16_t)acc_X;
    acc_Y_out = (uint16_t)acc_Y;
    acc_Z_out = (uint16_t)acc_Z;

    
//- угол для каждой оси
    deg_X = acc_X/8192.0;
    if(deg_X > 1)  deg_X = 1;
    if(deg_X < -1) deg_X = -1;
    ang_X = (int16_t)(asin(deg_X)*180.0/3.14);
    angleX = (uint8_t)(ang_X + 125);
    
    deg_Y = acc_Y/8192.0;
    if(deg_Y > 1)  deg_Y = 1;
    if(deg_Y < -1) deg_Y = -1;
    ang_Y = (int16_t)(asin(deg_Y)*180.0/3.14);
    angleY = (uint8_t)(ang_Y + 125);
    
    deg_Z = acc_Z/8192.0;
    if(deg_Z > 1)  deg_Z = 1;
    if(deg_Z < -1) deg_Z = -1;
    ang_Z = (int16_t)(asin(deg_Z)*180.0/3.14);
    angleZ = (uint8_t)(ang_Z + 125);

//- косинус угла и сам угол между векторами
    int32_t acc_X0 = Ecuinfo.params.BMA456.acc_X0;
    int32_t acc_Y0 = Ecuinfo.params.BMA456.acc_Y0;
    int32_t acc_Z0 = Ecuinfo.params.BMA456.acc_Z0;
    int32_t acc_XYZ = acc_X0*acc_X0+acc_Y0*acc_Y0+acc_Z0*acc_Z0;
    
    cos_angle = -(acc_X0*acc_X + acc_Y0*acc_Y + acc_Z0*acc_Z)/(sqrt((float)(acc_XYZ))*sqrt((float)(acc_X*acc_X+acc_Y*acc_Y+acc_Z*acc_Z)));

    ang_Fi = (uint8_t)(acos(-cos_angle)*180.0/3.14);

  }

}