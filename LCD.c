/*--------------------------------------------------------------------------------------------------------------------------
 * file name  : lcd.c
 * Author     : OUN
 * Created on : July 23, 2021
 * description: lcd source file driver
 --------------------------------------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------INCLUDES------------------------------------------------------------*/

#include "LCD.h"

/*--------------------------------------------LOCAL FUNCTION DECLARATIONS--------------------------------------------------*/
void set_LCD_CTRL_PCTL(uint8 pin) {
	switch (pin) {
	case 0:
		LCD_CTRL_PROT_PCTL &= 0xFFFFFFF0;
		break;
	case 1:
		LCD_CTRL_PROT_PCTL &= 0xFFFFFF0F;
		break;
	case 2:
		LCD_CTRL_PROT_PCTL &= 0xFFFFF0FF;
		break;
	case 3:
		LCD_CTRL_PROT_PCTL &= 0xFFFF0FFF;
		break;
	case 4:
		LCD_CTRL_PROT_PCTL &= 0xFFF0FFFF;
		break;
	case 5:
		LCD_CTRL_PROT_PCTL &= 0xFF0FFFFF;
		break;
	case 6:
		LCD_CTRL_PROT_PCTL &= 0xF0FFFFFF;
		break;
	case 7:
		LCD_CTRL_PROT_PCTL &= 0x0FFFFFFF;
		break;
	}
}

void Delay_MS(unsigned long long n)
{
	volatile unsigned long long count = 0;
	while (count++ < (NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND * n));
}

/*-----------------------------------------------FUNCTION DECLARATIONS-----------------------------------------------------*/

void LCD_init(void) {

	/* Enable clock for and allow time for clock to start*/
	volatile unsigned long delay = 0;
	SYSCTL_REGCGC2_REG |= (1 << LCD_CTRL_PORT) | (1 << LCD_DATA_PORT);  /* enabel clock for GPIO_PORTA*/
	delay = SYSCTL_REGCGC2_REG;


	LCD_CTRL_PORT_AMSEL &= ~(1 << RS) & ~(1 << RW) & ~(1 << E);   /* disable analog mode*/
	set_LCD_CTRL_PCTL(RS);							              /* Clear PMCx bits for RS,RW and E to use it as GPIO pin */
	set_LCD_CTRL_PCTL(RW);
	set_LCD_CTRL_PCTL(E);
	LCD_CTRL_PORT_DIR |= (1 << RS) | (1 << RW) | (1 << E);        /* set pins RS and RW and E as output pins */
	LCD_CTRL_PORT_AFSEL &= ~(1 << RS) & ~(1 << RW) & ~(1 << E);   /* deactivate alternate functions*/
	LCD_CTRL_PORT_DEN |= (1 << RS) | (1 << RW) | (1 << E);        /* enable digital pin*/
	LCD_CTRL_PORT_OUT &= ~(1 << RS) & ~(1 << RW) & ~(1 << E);    /* Clear bits in Data regsiter to intial value 0 */

#if (DATA_BITS_MODE == 8)
	LCD_DATA_PORT_AMSEL &= 0xFFFFFF00;            /* disable analog mode*/
	LCD_DATA_PROT_PCTL = 0x00;          /* Clear PMCx bits for PA5 to use it as GPIO pin */
	LCD_DATA_PORT_DIR |= 0xFF;                 /* set PA5 as output pin */
	LCD_DATA_PORT_AFSEL &= 0xFFFFFF00;              /* deactivate alternate functions*/
	LCD_DATA_PORT_DEN |= 0xFF;                 /* enable digital pin*/
	LCD_DATA_PORT_OUT &= 0xFFFFFF00;               /* Clear bit 5 in Data regsiter to OUT 0 */
	LCD_sendCommand(TWO_LINE_LCD_EIGHT_BIT_MODE);
#elif (DATA_BITS_MODE == 4)
#ifdef LOWER_PORT_PINS
	LCD_DATA_PORT_AMSEL &= 0xFFFFFFF0;            /* disable analog mode*/
	LCD_DATA_PROT_PCTL &= 0xFFFF0000;          /* Clear PMCx bits for PA5 to use it as GPIO pin */
	LCD_DATA_PORT_DIR |= 0x0F;                 /* set PA5 as output pin */
	LCD_DATA_PORT_AFSEL &= 0xFFFFFFF0;              /* deactivate alternate functions*/
	LCD_DATA_PORT_DEN |= 0x0F;                 /* enable digital pin*/
	LCD_DATA_PORT_OUT &= 0xFFFFFFF0;               /* Clear bit 5 in Data regsiter to OUT 0 */
#else
	LCD_DATA_PORT_AMSEL &= 0xFFFFFF0F;            /* disable analog mode*/
	LCD_DATA_PROT_PCTL &= 0x0000FFFF;          /* Clear PMCx bits for PA5 to use it as GPIO pin */
	LCD_DATA_PORT_DIR |= 0xF0;                 /* set PA5 as output pin */
	LCD_DATA_PORT_AFSEL &= 0xFFFFFF0F;              /* deactivate alternate functions*/
	LCD_DATA_PORT_DEN |= 0xF0;                 /* enable digital pin*/
	LCD_DATA_PORT_OUT &= 0xFFFFFF0F;               /* Clear bit 5 in Data regsiter to OUT 0 */
#endif
	LCD_sendCommand(FOUR_BITS_DATA_MODE);
	LCD_sendCommand(TWO_LINE_LCD_FOUR_BIT_MODE);
#endif

	LCD_sendCommand(CURSOT_OFF);
	LCD_sendCommand(CLEAR_SCREEN);
}


void LCD_sendCommand(uint8 command){
	CLEAR_BIT(LCD_CTRL_PORT_OUT,RS);
	CLEAR_BIT(LCD_CTRL_PORT_OUT,RW);
	Delay_MS(1);
	SET_BIT(LCD_CTRL_PORT_OUT,E);
	Delay_MS(1);
	#if (DATA_BITS_MODE == 8)

		LCD_DATA_PORT_OUT = command;
		Delay_MS(1);
		CLEAR_BIT(LCD_CTRL_PORT_OUT,E);
		Delay_MS(1);

	#elif (DATA_BITS_MODE == 4)
		#ifdef LOWER_PORT_PINS
			LCD_DATA_PORT_OUT = (LCD_DATA_PORT_OUT & 0xF0) | ((command & 0xF0) >> 4);
		#else
			LCD_DATA_PORT_OUT = (LCD_DATA_PORT_OUT & 0x0F) | (command & 0xF0);
		#endif

			Delay_MS(1);
			CLEAR_BIT(LCD_CTRL_PORT_OUT,E);
			Delay_MS(1);
			SET_BIT(LCD_CTRL_PORT_OUT,E);
			Delay_MS(1);

		#ifdef LOWER_PORT_PINS
			LCD_DATA_PORT_OUT = (LCD_DATA_PORT_OUT & 0xF0) | (command & 0x0F);
		#else
			LCD_DATA_PORT_OUT = (LCD_DATA_PORT_OUT & 0x0F) | ((command & 0x0F) << 4);
		#endif

			Delay_MS(1);
			CLEAR_BIT(LCD_CTRL_PORT_OUT,E);
			Delay_MS(1);

	#endif


}

void LCD_goToRowColumn(uint8 row,uint8 col){
	switch(row)
	{
	case 0:
		LCD_sendCommand( col | SET_CURSOR_LOCATION);
		break;
	case 1:
		LCD_sendCommand( (col+0x40) | SET_CURSOR_LOCATION);
		break;
	case 2:
		LCD_sendCommand( (col+0x10) | SET_CURSOR_LOCATION);
		break;
	case 3:
		LCD_sendCommand( (col+0x50) | SET_CURSOR_LOCATION);
		break;
	}

}

void LCD_displayCharacter(uint8 character){
	SET_BIT(LCD_CTRL_PORT_OUT,RS);
	CLEAR_BIT(LCD_CTRL_PORT_OUT,RW);
	Delay_MS(1);
	SET_BIT(LCD_CTRL_PORT_OUT,E);
	Delay_MS(1);

	#if (DATA_BITS_MODE == 8)

		LCD_DATA_PORT_OUT = character;
		_delay_ms(1);
		CLEAR_BIT(LCD_CTRL_PORT_OUT,E);
		_delay_ms(1);

	#elif (DATA_BITS_MODE == 4)
		#ifdef LOWER_PORT_PINS
			LCD_DATA_PORT_OUT = (LCD_DATA_PORT_OUT & 0xF0) | ((character & 0xF0) >> 4);
		#else
			LCD_DATA_PORT_OUT = (LCD_DATA_PORT_OUT & 0x0F) | (character & 0xF0);
		#endif

		Delay_MS(1);
		CLEAR_BIT(LCD_CTRL_PORT_OUT,E);
		Delay_MS(1);
		SET_BIT(LCD_CTRL_PORT_OUT,E);
		Delay_MS(1);

		#ifdef LOWER_PORT_PINS
			LCD_DATA_PORT_OUT = (LCD_DATA_PORT_OUT & 0xF0) | (character & 0x0F);
		#else
			LCD_DATA_PORT_OUT = (LCD_DATA_PORT_OUT & 0x0F) | ((character & 0x0F) << 4);
		#endif

		Delay_MS(1);
		CLEAR_BIT(LCD_CTRL_PORT_OUT,E);
		Delay_MS(1);

	#endif
}

void LCD_displayCharacterRowColumn(uint8 row,uint8 col, uint8 character){
	LCD_goToRowColumn(row, col);
	LCD_displayCharacter(character);
}

void LCD_displayString(const char *string){
	uint8 i = 0;
	while(string[i] != '\0'){
		LCD_displayCharacter(string[i]);
		i++;
	}
}
void LCD_displayStringRowColumn(uint8 row, uint8 col, const char *string){
	LCD_goToRowColumn(row, col);
	LCD_displayString(string);
}
void LCD_intgerToString(int data){
	char buff[16];
	sprintf(buff, "%d", data);
	LCD_displayString(buff);
}
void LCD_clearScreen(void){
	LCD_sendCommand(CLEAR_SCREEN);
}
void LCD_clearRow(uint8 row){
	uint8 i;
	switch(row)
	{
	case 0:
		LCD_sendCommand( (0x00) |SET_CURSOR_LOCATION);
		for (i = 0 ; i <= 0x0F ; i++){
			LCD_displayCharacter(' ');
		}
		break;
	case 1:
		LCD_sendCommand( (0x40) | SET_CURSOR_LOCATION);
		for (i = 0x40 ; i <= 0X4F ; i++){
			LCD_displayCharacter(' ');
		}
		break;
	case 2:
		LCD_sendCommand( (0x10) | SET_CURSOR_LOCATION);
		for (i = 0x10 ; i <= 0X1F ; i++){
			LCD_displayCharacter(' ');
		}
		break;
	case 3:
		LCD_sendCommand( (0x50) | SET_CURSOR_LOCATION);
		for (i = 0x50 ; i <= 0X5F ; i++){
			LCD_displayCharacter(' ');
		}
		break;
	}
	LCD_goToRowColumn(row, 0);
}

void LCD_clearRowColumn(uint8 row, uint8 col){
	LCD_goToRowColumn(row, col);
	LCD_displayCharacter(' ');
}


/** warning !!!! **/
/* infinity loop */

/**
void LCD_displayStringShiftLift(uint8 row, const char *string) {
	uint8 i = 0, j = 0, length = 0;
	LCD_clearRow(row);
	if (strlen(string) > 16) {
		length = strlen(string);
	}
	else {
		length = 16;
	}
	while (1) {
          i = 0;
		for (i = length; i > 0; i--) {
			j = length - i;
			LCD_displayStringRowColumn(row, i, string);
			LCD_displayStringRowColumn(row, 0, &string[j]);
			Delay_MS(1000);
		}
	}
}

**/