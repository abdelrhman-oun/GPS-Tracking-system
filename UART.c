/*--------------------------------------------------------------------------------------------------------------------------
* file name  : UART.c
* Author     : OUN
* Created on : July 23, 2021
* description: UART source file driver for TM4CF MCU
--------------------------------------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------INCLUDES------------------------------------------------------------*/

#include "UART.h"

/*----------------------------------------------------DFINATIONS-----------------------------------------------------------*/

#define CTL_ENABLE        0x0301			 /* ENABLE UART , ENABLE TRANSMITTER ,ENABLE RECEIVER */

/*-----------------------------------------------FUNCTIONS DEFINITIONS-----------------------------------------------------*/

void UART_init(Uart_ConfigType * ConfigPtr) {

	volatile uint32 * UART_Ptr = NULL_PTR;        /* point to the required UART Registers base address */

	SYSCTL_RCGCUART_R |= (1<<ConfigPtr->uart_num);                                 /* ACTIVATE UART CLOCK */

	switch (ConfigPtr->uart_num)
	{
	case  0:
		UART_Ptr = (volatile uint32 *)UART0_BASE_ADDRESS; /* UART0 Base Address */
		break;
	case  1:
		UART_Ptr = (volatile uint32 *)UART1_BASE_ADDRESS; /* UART1 Base Address */
		break;
	case  2:
		UART_Ptr = (volatile uint32 *)UART2_BASE_ADDRESS; /* UART2 Base Address */
		break;
	case  3:
		UART_Ptr = (volatile uint32 *)UART3_BASE_ADDRESS; /* UART3 Base Address */
		break;
	case  4:
		UART_Ptr = (volatile uint32 *)UART4_BASE_ADDRESS; /* UART4 Base Address */
		break;
	case  5:
		UART_Ptr = (volatile uint32 *)UART5_BASE_ADDRESS; /* UART5 Base Address */
		break;
	case  6:
		UART_Ptr = (volatile uint32 *)UART6_BASE_ADDRESS; /* UART6 Base Address */
		break;
	case  7:
		UART_Ptr = (volatile uint32 *)UART7_BASE_ADDRESS; /* UART7 Base Address */
		break;
	}

	/* disable UART*/
	*(volatile uint32 *)((volatile uint8 *)UART_Ptr + UART_CTRL_REG_OFFSET) = 0;

	/* UART-_IBRD_R  , MASKING ISN'T NESSECERALLY NEEDED HERE VALUE IS VERY LOW IN COMPARSION */
	*(volatile uint32 *)((volatile uint8 *)UART_Ptr + UART_INT_BAUD_RATE_REG_OFFSET) = ConfigPtr->int_div;

	/* UART-_FBRD_R */
	*(volatile uint32 *)((volatile uint8 *)UART_Ptr + UART_FRAC_BAUD_RATE_REG_OFFSET) = ConfigPtr->frac_div;

	/* UART configuration parity, no. of stop bit and no. of data bits */
	/* parity */
	*(volatile uint32 *)((volatile uint8 *)UART_Ptr + UART_LINE_CTRL_REG_OFFSET) = (*(volatile uint32 *)((volatile uint8 *)UART_Ptr + UART_LINE_CTRL_REG_OFFSET) & (0xFFFFFFF9)) | ((ConfigPtr->parity) << 1);
	/* stop bit */
	if (ConfigPtr->stopBit == SINGLE) {
		CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)UART_Ptr + UART_LINE_CTRL_REG_OFFSET), 3);
	}
	else {
		SET_BIT(*(volatile uint32 *)((volatile uint8 *)UART_Ptr + UART_LINE_CTRL_REG_OFFSET), 3);
	}
	/* no. of data bits*/
	*(volatile uint32 *)((volatile uint8 *)UART_Ptr + UART_LINE_CTRL_REG_OFFSET) = (*(volatile uint32 *)((volatile uint8 *)UART_Ptr + UART_LINE_CTRL_REG_OFFSET) & (0xFFFFFF9F)) | ((ConfigPtr->bitDataNum) << 5);

	/* ENABLE UART , ENABLE TRANSMITTER ,ENABLE RECEIVER */
	*(volatile uint32 *)((volatile uint8 *)UART_Ptr + UART_CTRL_REG_OFFSET) = CTL_ENABLE;

}


void UART_sendByte(const uint8 uart_num, const uint8 data) {

	volatile uint32 * UART_Ptr = NULL_PTR;        /* point to the required UART Registers base address */

	switch (uart_num)
	{
	case  0:
		UART_Ptr = (volatile uint32 *)UART0_BASE_ADDRESS; /* UART0 Base Address */
		break;
	case  1:
		UART_Ptr = (volatile uint32 *)UART1_BASE_ADDRESS; /* UART1 Base Address */
		break;
	case  2:
		UART_Ptr = (volatile uint32 *)UART2_BASE_ADDRESS; /* UART2 Base Address */
		break;
	case  3:
		UART_Ptr = (volatile uint32 *)UART3_BASE_ADDRESS; /* UART3 Base Address */
		break;
	case  4:
		UART_Ptr = (volatile uint32 *)UART4_BASE_ADDRESS; /* UART4 Base Address */
		break;
	case  5:
		UART_Ptr = (volatile uint32 *)UART5_BASE_ADDRESS; /* UART5 Base Address */
		break;
	case  6:
		UART_Ptr = (volatile uint32 *)UART6_BASE_ADDRESS; /* UART6 Base Address */
		break;
	case  7:
		UART_Ptr = (volatile uint32 *)UART7_BASE_ADDRESS; /* UART7 Base Address */
		break;
	}

	while (BIT_IS_CLEAR(*(volatile uint32 *)((volatile uint8 *)UART_Ptr + UART_FLAG_REG_OFFSET),5));   //POLLING UNTIL TX FULL FLAG =0 (IF DATA BUFFER ISN'T FULL WE CAN SEND DATA) 
	*(volatile uint32 *)((volatile uint8 *)UART_Ptr + UART_DATA_REG_OFFSET) = data;
}

uint8 UART_receiveByte(const uint8 uart_num) {

	volatile uint32 * UART_Ptr = NULL_PTR;        /* point to the required UART Registers base address */

	switch (uart_num)
	{
	case  0:
		UART_Ptr = (volatile uint32 *)UART0_BASE_ADDRESS; /* UART0 Base Address */
		break;
	case  1:
		UART_Ptr = (volatile uint32 *)UART1_BASE_ADDRESS; /* UART1 Base Address */
		break;
	case  2:
		UART_Ptr = (volatile uint32 *)UART2_BASE_ADDRESS; /* UART2 Base Address */
		break;
	case  3:
		UART_Ptr = (volatile uint32 *)UART3_BASE_ADDRESS; /* UART3 Base Address */
		break;
	case  4:
		UART_Ptr = (volatile uint32 *)UART4_BASE_ADDRESS; /* UART4 Base Address */
		break;
	case  5:
		UART_Ptr = (volatile uint32 *)UART5_BASE_ADDRESS; /* UART5 Base Address */
		break;
	case  6:
		UART_Ptr = (volatile uint32 *)UART6_BASE_ADDRESS; /* UART6 Base Address */
		break;
	case  7:
		UART_Ptr = (volatile uint32 *)UART7_BASE_ADDRESS; /* UART7 Base Address */
		break;
	}

	while (BIT_IS_CLEAR(*(volatile uint32 *)((volatile uint8 *)UART_Ptr + UART_FLAG_REG_OFFSET), 4));   //POLLING UNTIL RX EMPTY FLAG =0 ( DATA BUFFER ISN'T EMPTY SO WE CAN START RECEIVING DATA)
	return (uint8) *(volatile uint32 *)((volatile uint8 *)UART_Ptr + UART_DATA_REG_OFFSET);  // RETURN FIRST 8 BITS IN UART0 DATA REGISTER
}