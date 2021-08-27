/*--------------------------------------------------------------------------------------------------------------------------
* file name  : UART.h
* Author     : OUN
* Created on : July 23, 2021
* description: UART header file driver for TM4CF MCU
--------------------------------------------------------------------------------------------------------------------------*/

#ifndef UART_H_
#define UART_H_

/*-----------------------------------------------------INCLUDES------------------------------------------------------------*/

#include "UART_REG.h"
#include "tm4c123gh6pm_registers.h"
#include "Platform_Types.h"
#include "Common_Macros.h"

/*-------------------------------------------------TYPES DECLARATION-------------------------------------------------------*/

typedef enum
{
	NO_PARITY , EVEN , ODD = 3
}Uart_ParityType;

typedef enum
{
	SINGLE, DOUBLE
}Uart_StopBitNum;

typedef enum
{
	FIVE_BITS, SIX_BITS, SEVEN_BITS, EIGHT_BITS
}Uart_DataNum;


typedef struct
{
	uint8 uart_num;
	uint16 int_div;
	uint8 frac_div;
	Uart_ParityType parity;
	Uart_StopBitNum stopBit;
	Uart_DataNum bitDataNum;
}Uart_ConfigType;



/*-------------------------------------------------TYPES DECLARATION-------------------------------------------------------*/

void UART_init(Uart_ConfigType * ConfigPtr);

void UART_sendByte(const uint8 uart_num ,const uint8 data);

uint8 UART_receiveByte(const uint8 uart_num);





#endif /* UART_H_ */
