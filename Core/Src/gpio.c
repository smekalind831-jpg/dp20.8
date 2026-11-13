/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
#include "Defs1.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1  */


/* USER CODE END 1 */

/* Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_SetOutputPin(GPIOA, CanEN_Pin|LIN_Pin);

  /**/
  LL_GPIO_SetOutputPin(GPIOB, CS1_Pin|CS2_Pin|MS_Pin|HOLD_Pin);
   
  /**/
  GPIO_InitStruct.Pin = CanEN_Pin|LIN_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;  //\ 50 MGc
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  GPIOA->BSRR = CanEN_Pin | LIN_Pin;
  
  /**/
   GPIO_InitStruct.Pin = CS1_Pin|CS2_Pin|MS_Pin|HOLD_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;  //\ 50 MGc
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Прерывание */
  LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTB, LL_GPIO_AF_EXTI_LINE12); 

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_12;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING_FALLING; //\ прерывание по двум фронтам
  //EXTI->RTSR |= 0x00001000;                               //\ EXTI->RTSR - запись 1 для положит перепада
  //EXTI->FTSR |= 0x00001000;                               //\ EXTI->FTSR - отрицательного
  EXTI->IMR = 0x01000;                                    //\ включить прерывание
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  LL_GPIO_SetPinPull(Off_Port, Off_In, LL_GPIO_PULL_UP);   // подтяжка вверх

  /**/
  LL_GPIO_SetPinMode(Off_Port, Off_In, LL_GPIO_MODE_INPUT);

  /* EXTI interrupt init*/
  NVIC_SetPriority(EXTI15_10_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(EXTI15_10_IRQn);

  GPIOB->BSRR = CS1_Pin| CS2_Pin| 0x0400| 0x800; //MS_Pin| HOLD_Pin;   //\ hold у FRAM всегда 1
  CanEN_GPIO_Port->BSRR = CanEN_Pin << 16| LIN_Pin; 
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
