/** 
THE MAIN APPLICATION that receive the coordenate from GPS module
and calculate the distance from starting untill ending

* Author     : OUN
* Created on : July 25, 2021
* description: Application main file to get coordinates from GPS module and calculate the total distance walked

**/


/*-----------------------------------------------------INCLUDES------------------------------------------------------------*/

#include <math.h>
#include "LCD.h"
#include "UART.h"

/*----------------------------------------------------DFINATIONS-----------------------------------------------------------*/

/* UART Driver Baud Rate */
#define UART_BAUDRATE 9600
#define UART_DIVINT_VALUE 104
#define UART_DIVFRAC_VALUE 11

#define UART_PORT 4                  /* port E */
#define UART_RX 4                    /* pin 4 used as receive */
#define UART_TX 5                    /* pin 5 used as transemit */

#define NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECON 762

#define pi 3.14159265358979323846


/*-------------------------------------------------GLOBAL VARIABLES--------------------------------------------------------*/

float64 GPS_oldCoo[2] = { 0 }, GPS_newCoo[2] = { 0 };
uint8 SW2_flag = 0;

/* UART confuguratino structure */
Uart_ConfigType config_struct = {
	5,
	UART_DIVINT_VALUE,
	UART_DIVFRAC_VALUE,
	NO_PARITY,
	SINGLE,
	EIGHT_BITS,
};


/*-----------------------------------------------FUNCTION DECLARATIONS-----------------------------------------------------*/

/* function to give a delay in millisecond */
void Delay_ms(unsigned long long n)
{
	volatile unsigned long long count = 0;
	while (count++ < (NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECON * n));
}

/* function to initialize GPIO pins to work with UART */
void UART_pinsInit(void) {

	volatile unsigned long dd = 0;
	SYSCTL_REGCGC2_REG |= (1 << UART_PORT);                                /* ACTIVATE CLOCK FOR PORT E */
	dd = SYSCTL_REGCGC2_REG;

	GPIO_PORTE_AMSEL_REG &= ~(1 << UART_RX) & ~(1 << UART_TX);             /* WE WANT THIS PIN DIGITAL -> GPIO_PORTE_AMSEL_REG */
	GPIO_PORTE_AFSEL_REG |= (1 << UART_RX) | (1 << UART_TX);               /* TO ACTIVATE ALTERNATIVE FUNCTION SELECT FOR PIN4 AND PIN5 */
	GPIO_PORTE_PCTL_REG |= 0X00110000;                                     /* CHOOSE PIN4 AND PIN5 TO BE UART5 RECEIVE AND TRANSMIT RESPECTIVELY */
	GPIO_PORTE_DEN_REG |= (1 << UART_RX) | (1 << UART_TX);                 /* TO ENABLE PIN TO BE DIGITAL */
}

/* This function converts decimal degrees to radians */
float64 deg2rad(float64 deg) {
	return (deg * pi / 180);
}

/* This function converts radians to decimal degrees */
float64 rad2deg(float64 rad) {
	return (rad * 180 / pi);
}

/* function to calculate the distance from two coordinates and return the distance in different units */
float64 distance(float64 lat1, float64 lon1, float64 lat2, float64 lon2, uint8 unit) {
	float64 theta, dist;
	theta = lon1 - lon2;
	dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
	dist = acos(dist);
	dist = rad2deg(dist);
	dist = dist * 60 * 1.1515;
	switch (unit) {
	case 'M':
		break;
	case 'K':
		dist = dist * 1.609344;
		break;
	case 'm':
		dist = (dist * 1.609344) / 1000;
		break;
	case 'N':
		dist = dist * 0.8684;
		break;
	}
	return (dist);
}

/* function receive data from gps module and extract coordinats form it then save the new coordinate in GPS_oldCoo[] */
void GPS_getCoordinates(void) {
	sint16 Gpsdata;                    /* for incoming serial data */
	uint16 finish = 0;                 /* indicate the end of message */ 
	uint16 pos_cnt = 0;                /* position counter */
	uint16 lat_cnt = 0;                /* latitude data counter */
	uint16 log_cnt = 0;                /* longitude data counter */
	uint16 flg = 0;                    /* GPS flag */
	uint16 com_cnt = 0;                /* comma counter */
	uint8 lat[20];                     /* latitude array */
	uint8 lg[20];                      /* longitude array */

	while (finish == 0) {
		// Check GPS data
		Gpsdata = UART_receiveByte(5);
		flg = 1;
		if (Gpsdata == '$' && pos_cnt == 0)   // finding GPRMC header
			pos_cnt = 1;
		if (Gpsdata == 'G' && pos_cnt == 1)
			pos_cnt = 2;
		if (Gpsdata == 'P' && pos_cnt == 2)
			pos_cnt = 3;
		if (Gpsdata == 'R' && pos_cnt == 3)
			pos_cnt = 4;
		if (Gpsdata == 'M' && pos_cnt == 4)
			pos_cnt = 5;
		if (Gpsdata == 'C' && pos_cnt == 5)
			pos_cnt = 6;
		if (Gpsdata == ',' && pos_cnt == 6) {   // count commas in message
			com_cnt++;
			flg = 0;
		}

		if (com_cnt == 3 && flg == 1) {
			lat[lat_cnt++] = Gpsdata;         // latitude
			flg = 0;
		}

		if (com_cnt == 5 && flg == 1) {
			lg[log_cnt++] = Gpsdata;         // Longitude
			flg = 0;
		}

		if (Gpsdata == '*' && com_cnt >= 6) {
			com_cnt = 0;                      // end of GPRMC message
			lat_cnt = 0;
			log_cnt = 0;
			flg = 0;
			finish = 1;

		}
	}
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, lg);
	LCD_displayStringRowColumn(1, 0, lat);
	Delay_ms(9000);
}

/*---------------------------------------------------MAIN FUNCTION---------------------------------------------------------*/
int main()
{
	uint8 counter = 60;
	uint16 total_distance = 0;
        
	UART_init(&config_struct);
	UART_pinsInit();
	//UART1_init();


	LCD_init();
	LCD_clearScreen();
	LCD_displayString("system starting..");
	LCD_displayStringRowColumn(1,0,"GPS starting ");
	Delay_ms(3000);
	LCD_clearScreen();
	LCD_displayString("wait...");
	LCD_goToRowColumn(1, 0);
	while (counter != 0) {
		counter--;
		if (counter == 9) {
			LCD_clearRow(1);
		}
                LCD_goToRowColumn(1, 0);
		LCD_intgerToString((int)counter);
		Delay_ms(1000);
	}
	LCD_clearScreen();
	LCD_displayString("system is ready !");

	GPS_getCoordinates();
	GPS_oldCoo[0] = GPS_newCoo[0];
	GPS_oldCoo[1] = GPS_newCoo[1];
	total_distance = (uint16)distance(GPS_newCoo[0], GPS_newCoo[1], GPS_oldCoo[0], GPS_oldCoo[1], 'm');

	LCD_clearScreen();
	LCD_displayString("total distance :");
	LCD_goToRowColumn(1, 0);
	LCD_intgerToString(total_distance);
	LCD_displayCharacter('m');

	while (1) {

		if (SW2_flag == 1) {
			total_distance = 0;
			SW2_flag = 0;
		}
		
		Delay_ms(4000);
		GPS_getCoordinates();
		total_distance += (uint16)distance(GPS_newCoo[0], GPS_newCoo[1], GPS_oldCoo[0], GPS_oldCoo[1], 'm');
		LCD_clearRow(1);
		LCD_goToRowColumn(1, 0);
		LCD_intgerToString(total_distance);
		LCD_displayCharacter('m');
		GPS_oldCoo[0] = GPS_newCoo[0];
		GPS_oldCoo[1] = GPS_newCoo[1];
	}
}
