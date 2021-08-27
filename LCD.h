
/*--------------------------------------------------------------------------------------------------------------------------
 * file name  : lcd.h
 * Author     : OUN
 * Created on : July 23, 2021
 * description: lcd header file driver
 --------------------------------------------------------------------------------------------------------------------------*/


#ifndef LCD_H_
#define LCD_H_

/*-----------------------------------------------------INCLUDES------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Common_Macros.h"
#include "tm4c123gh6pm_registers.h"
#include "Platform_Types.h"

/*----------------------------------------------------DFINATIONS-----------------------------------------------------------*/
#define _OPEN_SYS_ITOA_EXT  /* used with function sprintf() to convert from integer to string */

#define DATA_BITS_MODE 4

#if (DATA_BITS_MODE == 4)
#define LOWER_PORT_PINS
#endif
/*PORT NUMBRING*/
#define PORTA 0
#define PORTB 1
#define PORTD 3
#define PORTE 4
#define PORTF 5
   
/* LCD hardware pins*/
/* LCD control pins*/
#define LCD_CTRL_PORT           PORTE
#define LCD_CTRL_PORT_AMSEL     GPIO_PORTE_AMSEL_REG
#define LCD_CTRL_PROT_PCTL      GPIO_PORTE_PCTL_REG
#define LCD_CTRL_PORT_DIR       GPIO_PORTE_DIR_REG
#define LCD_CTRL_PORT_AFSEL     GPIO_PORTE_AFSEL_REG
#define LCD_CTRL_PORT_DEN       GPIO_PORTE_DEN_REG
#define LCD_CTRL_PORT_OUT       GPIO_PORTE_DATA_REG
#define RS 3     /* number of pin in the port*/
#define RW 2     /* number of pin in the port*/
#define E  1     /* number of pin in the port*/

/* LCD data pins*/
#define LCD_DATA_PORT           PORTD
#define LCD_DATA_PORT_AMSEL     GPIO_PORTD_AMSEL_REG
#define LCD_DATA_PROT_PCTL      GPIO_PORTD_PCTL_REG
#define LCD_DATA_PORT_DIR       GPIO_PORTD_DIR_REG
#define LCD_DATA_PORT_AFSEL     GPIO_PORTD_AFSEL_REG
#define LCD_DATA_PORT_DEN       GPIO_PORTD_DEN_REG
#define LCD_DATA_PORT_OUT       GPIO_PORTD_DATA_REG

/* defination for daley function */
#define NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND 762 /*specific for tiva-C*/
   
/* LCD COMMANDS*/
#define CLEAR_SCREEN                0X01
#define RETURN_HOME                 0X02
#define SHIFT_CURSOR_LEFT           0X04
#define SHIFT_CUTSOR_RIGHT          0X06
#define SHIFT_DISPLAY_LEFT          0X07
#define SHIFT_DISPLAY_RIGHT         0X05
#define SHIFT_ENTIRE_DISPLAY_LEFT   0X18
#define SHIFT_ENTIRE_DISPLAY_RIGHT  0X1C
#define CURSOR_ON                   0X0E
#define CURSOT_OFF                  0X0C
#define SET_CURSOR_LOCATION         0x80
#define FOUR_BITS_DATA_MODE         0x02
#define TWO_LINE_LCD_FOUR_BIT_MODE  0x28
#define TWO_LINE_LCD_EIGHT_BIT_MODE 0x38

/*-----------------------------------------------FUNCTION DECLARATIONS-----------------------------------------------------*/

void LCD_init(void);
void LCD_sendCommand(uint8 command);
void LCD_goToRowColumn(uint8 row, uint8 col);
void LCD_displayCharacter(uint8 character);
void LCD_displayCharacterRowColumn(uint8 row,uint8 col, uint8 character);
void LCD_displayString(const char *string);
void LCD_displayStringRowColumn(uint8 row, uint8 col, const char *string);
void LCD_intgerToString(int data);
void LCD_clearScreen(void);
void LCD_clearRow(uint8 row);
void LCD_clearRowColumn(uint8 row, uint8 col);

/** warning !!!! **/
/* infinity loop */
/**
void LCD_displayStringShiftLift(uint8 row, const char *string);
**/





#endif /* LCD_H_ */
