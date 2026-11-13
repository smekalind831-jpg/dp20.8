/*!
 * @file       apm32f10x_gpio.c
 *
 * @brief      This file provides all the GPIO firmware functions
 *
 * @details   
 *
 * @version    V1.0.0
 * 
 * @date       2019-8-6
 *
 */

#include "apm32f10x_gpio.h"
#include "apm32f10x_rcm.h"

/*!
 * @brief     Reset GPIO peripheral registers to their default reset values
 *
 * @param     port   GPIO peripheral
 *
 * @retval    None
 *
 * @note 
 */
void GPIO_Reset(GPIO_T* port)
{
   RCM_APB2_PERIPH_T APB2Periph;

   if (port == GPIOA)
   {
      APB2Periph = RCM_APB2_PERIPH_GPIOA;
   }
   else if (port == GPIOB)
   {
      APB2Periph = RCM_APB2_PERIPH_GPIOB;
   }
   else if (port == GPIOC)
   {
      APB2Periph = RCM_APB2_PERIPH_GPIOC;
   }
   else if (port == GPIOD)
   {
      APB2Periph = RCM_APB2_PERIPH_GPIOD;
   }    
   else if (port == GPIOE)
   {
      APB2Periph = RCM_APB2_PERIPH_GPIOE;
   } 
   else if (port == GPIOF)
   {
      APB2Periph = RCM_APB2_PERIPH_GPIOF;
   }
   if (port == GPIOG)
   {
      APB2Periph = RCM_APB2_PERIPH_GPIOG;
   }

   RCM_EnableAPB2PeriphReset(APB2Periph);
   RCM_DisableAPB2PeriphReset(APB2Periph);   
}

/*!
 * @brief     Reset Alternate Functions registers to their default reset values
 *
 * @param     None
 *
 * @retval    None
 *
 * @note 
 */
void AFIO_Reset(void)
{
   RCM_EnableAPB2PeriphReset(RCM_APB2_PERIPH_AFIO);
   RCM_DisableAPB2PeriphReset(RCM_APB2_PERIPH_AFIO);   
}

/*!
 * @brief     Config the GPIO peripheral according to the specified parameters in the configStruct
 *
 * @param     port   GPIO peripheral
 *
 * @param     configStruct: pointer to a GPIO_ConfigStruct_T structure
 *
 * @retval    None
 *
 * @note 
 */
void GPIO_Config(GPIO_T* port, GPIO_ConfigStruct_T* configStruct)
{
   uint8_t i;
   uint32_t mode;
   uint32_t CR;
   uint32_t temp;
   uint32_t shift;
   
   mode = configStruct->mode & 0x0f;
   if(configStruct->mode & 0x80)
   {
      mode |= configStruct->speed;
   }

   if(configStruct->pin & 0xff)
   {
      CR = port->CTRL0;
      for(i = 0, shift = 0x01; i < 8; i++, shift <<= 1)
      {
         if(configStruct->pin & shift)
         {
            temp = i << 2;
            CR &= (uint32_t)~(0x0f << temp);
            CR |= mode << temp;
      
            if(configStruct->mode == GPIO_MODE_IN_PD)
            {
               port->BC = shift;
            }
            else if(configStruct->mode == GPIO_MODE_IN_PU)
            {
               port->BSC = shift;
            }
         }
      }
      port->CTRL0 = CR;
   }

   if(configStruct->pin & 0xff00)
   {
      CR = port->CTRL1;
      for(i = 8, shift = 0x100; i < 16; i++, shift <<= 1)
      {
         if(configStruct->pin & shift)
         {
            temp = (i - 8) << 2;
            CR &= (uint32_t)~(0x0f << temp);
            CR |= mode << temp;
      
            if(configStruct->mode == GPIO_MODE_IN_PD)
            {
               port->BC = shift;
            }
            else if(configStruct->mode == GPIO_MODE_IN_PU)
            {
               port->BSC = shift;
            }
         }
      }
      port->CTRL1 = CR;
   }
}

/*!
 * @brief     Fills each GPIO_InitStruct member with its default value
 *
 * @param     configStruct   pointer to a GPIO_ConfigStruct_T structure which will be initialized
 *
 * @retval    None
 *
 * @note 
 */
void GPIO_StructInit(GPIO_ConfigStruct_T* configStruct)
{
   configStruct->pin  = GPIO_PIN_All;
   configStruct->speed = GPIO_SPEED_2MHz;
   configStruct->mode = GPIO_MODE_IN_FLOATING;
}

/*!
 * @brief     Reads the specified input port pin
 *
 * @param     port   GPIO peripheral
 *
 * @param     pin      specifies pin to read
 *
 * @retval    The input port pin value 
 *
 * @note 
 */
uint8_t GPIO_ReadInputBit(GPIO_T* port, uint16_t pin)
{
   uint8_t ret;

   ret = (port->DIN & pin) ?  BIT_SET : BIT_RESET;

   return ret;
}

/*!
 * @brief     Reads the specified GPIO input data port
 *
 * @param     port   GPIO peripheral
 *
 * @retval    GPIO input data port value
 *
 * @note 
 */
uint16_t GPIO_ReadInputPort(GPIO_T* port)
{
  return ((uint16_t)port->DIN);
}

/*!
 * @brief     Reads the specified output data port bit
 *
 * @param     port   GPIO peripheral
 *
 * @param     pin      specifies pin to read
 *
 * @retval    The output port pin value
 *
 * @note 
 */
uint8_t GPIO_ReadOutputBit(GPIO_T* port, uint16_t pin)
{

   uint8_t ret;

   ret = (port->DOUT & pin) ? BIT_SET : BIT_RESET;

   return ret;
}

/*!
 * @brief     Reads the specified GPIO output data port
 *
 * @param     port   GPIO peripheral
 *
 * @retval    output data port value
 *
 * @note 
 */
uint16_t GPIO_ReadOutputPort(GPIO_T* port)
{
   return ((uint16_t)port->DOUT);
}

/*!
 * @brief     Sets the selected data port bits
 *
 * @param     port   GPIO peripheral
 *
 * @param     pin      specifies the port bits to be written
 *
 * @retval    None
 *
 * @note 
 */
void GPIO_SetBits(GPIO_T* port, uint16_t pin)
{
   port->BSC = (uint32_t)pin;
}

/*!
 * @brief     Clears the selected data port bits
 *
 * @param     port   GPIO peripheral
 *
 * @param     pin      specifies the port bits to be written
 *
 * @retval    None
 *
 * @note 
 */
void GPIO_ResetBits(GPIO_T* port, uint16_t pin)
{
   port->BC = (uint32_t)pin;
}


/*!
 * @brief     Writes data to the specified GPIO data port
 *
 * @param     port      GPIO peripheral
 *
 * @param     portValue   specifies the value to be written to the port output data register
 *
 * @retval    None
 *
 * @note 
 */
void GPIO_WriteOutputPort(GPIO_T* port, uint16_t portValue)
{
   port->DOUT = (uint32_t)portValue;
}

/*!
 * @brief     Locks GPIO Pins configuration registers
 *
 * @param     port   GPIO peripheral
 *
 * @param     pin      specifies the port bit to be written
 *
 * @retval    None
 *
 * @note 
 */
void GPIO_ConfigPinLock(GPIO_T* port, uint16_t pin)
{
   uint32_t val = 0x00010000;

   val  |= pin;
   /* Set LCKK bit */
   port->LOCK = val ;
   /* Reset LCKK bit */
   port->LOCK =  pin;
   /* Set LCKK bit */
   port->LOCK = val;
   /* Read LCKK bit*/
   val = port->LOCK;
   /* Read LCKK bit*/
   val = port->LOCK;
}

/*!
 * @brief     Selects the GPIO pin used as Event output
 *
 * @param     portSource   selects the GPIO port to be used as source for Event output
 *
 * @param     pinSource   specifies the pin for the Event output
 *
 * @retval    None
 *
 * @note 
 */
void GPIO_ConfigEventOutput(GPIO_PORT_SOURCE_T portSource, GPIO_PIN_SOURCE_T pinSource)
{
   AFIO->EVCTRL_B.PORTSEL =  portSource;
   AFIO->EVCTRL_B.PINSEL = pinSource;
}

/*!
 * @brief     Enables the Event Output
 *
 * @param     None
 *
 * @retval    None
 *
 * @note 
 */
void GPIO_EnableEventOutput(void)
{
   AFIO->EVCTRL_B.EVOEN = BIT_SET;
}
/*!
 * @brief     Disable the Event Output
 *
 * @param     None
 *
 * @retval    None
 *
 * @note 
 */
void GPIO_DisableEventOutput(void)
{
   AFIO->EVCTRL_B.EVOEN = BIT_RESET;
}

/*!
 * @brief     Changes the mapping of the specified pin
 *
 * @param     remap   selects the pin to remap
 *
 * @retval    None
 *
 * @note 
 */
void GPIO_ConfigPinRemap(GPIO_REMAP_T remap)
{
   uint32_t val, mask, bitOffset, regOffset;
   uint32_t regVal;
   
   val = remap & 0x0f;
   mask = (remap >> 4) & 0x0f;
   bitOffset = (remap >> 8) & 0xff;
   regOffset = (remap >> 16) & 0x0f;

   if(regOffset)
   {
      regVal = AFIO->REMAP2;
   }
   else
   {
      regVal = AFIO->REMAP;
   }

   mask <<= bitOffset;
   regVal &= (uint32_t)~mask;
   val <<= bitOffset;
   regVal |= val;

   if(regOffset)
   {
      AFIO->REMAP2 = regVal;
   }
   else
   {
      AFIO->REMAP = regVal;
   }
}

/*!
 * @brief     Selects the GPIO pin used as EXTI Line
 *
 * @param     portSource   selects the GPIO port to be used as source for EXTI lines   
 *
 * @param     pinSource   specifies the EXTI line to be configured
 *
 * @retval    None
 *
 * @note 
 */
void GPIO_ConfigEINTLine(GPIO_PORT_SOURCE_T portSource, GPIO_PIN_SOURCE_T pinSource)
{
   uint32_t shift;
   
   if(pinSource <= GPIO_PIN_SOURCE_3)
   {
      shift = pinSource << 2;
      AFIO->EINTCFG1 &= (uint32_t )~(0x0f << shift);
      AFIO->EINTCFG1 |=  portSource << shift;
   }

   else if(pinSource <= GPIO_PIN_SOURCE_7)
   {
      shift = (pinSource - GPIO_PIN_SOURCE_4) << 2;
      AFIO->EINTCFG2 &= (uint32_t )~(0x0f << shift);
      AFIO->EINTCFG2 |=  portSource << shift;
   }   

   else if(pinSource <= GPIO_PIN_SOURCE_11)
   {
      shift = (pinSource - GPIO_PIN_SOURCE_8) << 2;
      AFIO->EINTCFG3 &= (uint32_t )~(0x0f << shift);
      AFIO->EINTCFG3 |=  portSource << shift;
   }   

   else if(pinSource <= GPIO_PIN_SOURCE_15)
   {
      shift = (pinSource - GPIO_PIN_SOURCE_12) << 2;
      AFIO->EINTCFG4 &= (uint32_t )~(0x0f << shift);
      AFIO->EINTCFG4 |=  portSource << shift;
   }      
}

