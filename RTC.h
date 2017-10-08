/*
 *
 *  Description:
 *  -------------
 *
 *    project name: RTC 
 *    File Name: RTC.h
 *
 *
 *    Version:		Date:        Author:         comment:
 *    --------		-----------  --------------  -------------------
 *    1.00			07.10.2017   Kevin Perillo   build.
 *
 *
 *    Copyright (c) MootSeeker 2017
/*=========================================================================*/

/* 
	This is a Header-File for the DS3231 Real Time Clock Module from Adafruit.
*/

#ifndef __RTC_H__
#define __RTC_H__

	#include "Arduino.h"
	#include <Wire.h>

/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
    #define Clock_ADDRESS                (0x68)
/*=========================================================================*/

/*=========================================================================
    REGISTERS
    -----------------------------------------------------------------------*/
	#define DS3231_REGISTER_Second   = 0x00, // Seconds Register
	#define DS3231_REGISTER_Minute   = 0x01, // Minutes Register
	#define DS3231_REGISTER_Hour	 = 0x02, // Hours Register
	#define DS3231_REGISTER_DoW		 = 0x03, // Day of the Week Register
	#define DS3231_REGISTER_Date	 = 0x04, // Date (Day) Register
	#define DS3231_REGISTER_Month	 = 0x05, // Month Register
	#define DS3231_REGISTER_Year	 = 0x06, // Year Register
	#define DS3231_REGISTER_Control  = 0x0E, // Control Register
	#define DS3231_REGISTER_MSB_Temp = 0x11, // MSB TEMP Register
	#define DS3231_REGISTER_LSB_Temp = 0x12  // LSB TEMP Register
  
/*=========================================================================*/

/*=========================================================================
    the get*() functions retrieve current values of the registers.
    -----------------------------------------------------------------------*/

	uint8_t getSecond(void); 
	uint8_t getMinute(void); 
	uint8_t getHour(bool& h12, bool& PM); 
		// In addition to returning the hour register, this function
		// returns the values of the 12/24-hour flag and the AM/PM flag.
	uint8_t getDoW(void); 
	uint8_t getDate(void); 
	uint8_t getMonth(bool& Century); 
		// Also sets the flag indicating century roll-over.
	uint8_t getYear(void); 
		// Last 2 digits only
			
/*=========================================================================*/

/*=========================================================================
    Time-setting functions.
    -----------------------------------------------------------------------*/

	// Note that none of these check for sensibility: You can set the
	// date to July 42nd and strange things will probably result.
	
	void setSecond(uint8_t Second); 
		// Sets the seconds
	void setMinute(uint8_t Minute); 
		// Sets the minute
	void setHour(uint8_t Hour); 
		// Sets the hour
	void setDoW(uint8_t DoW); 
		// Sets the Day of the Week (1-7);
	void setDate(uint8_t Date); 
		// Sets the Date of the Month
	void setMonth(uint8_t Month); 
		// Sets the Month of the year
	void setYear(uint8_t Year); 
		// Last two digits of the year
	void setClockMode(bool h12); 
		// Set 12/24h mode. True is 12-h, false is 24-hour.

/*=========================================================================*/

/*=========================================================================
    Temperature function.
    -----------------------------------------------------------------------*/

	float getTemperature(); 

/*=========================================================================*/

/*=========================================================================
    Alarm functions.
    -----------------------------------------------------------------------*/
		
	void getA1Time(uint8_t& A1Day, uint8_t& A1Hour, uint8_t& A1Minute, uint8_t& A1Second, 
					uint8_t& AlarmBits, bool& A1Dy, bool& A1h12, bool& A1PM); 
	
	/* Retrieves everything you could want to know about alarm
	 * one. 
	 * A1Dy true makes the alarm go on A1Day = Day of Week,
	 * A1Dy false makes the alarm go on A1Day = Date of month.
	 *
	 * byte AlarmBits sets the behavior of the alarms:
	 *	Dy	A1M4	A1M3	A1M2	A1M1	Rate
	 *	X	1		1		1		1		Once per second
	 *	X	1		1		1		0		Alarm when seconds match
	 *	X	1		1		0		0		Alarm when min, sec match
	 *	X	1		0		0		0		Alarm when hour, min, sec match
	 *	0	0		0		0		0		Alarm when date, h, m, s match
	 *	1	0		0		0		0		Alarm when DoW, h, m, s match
	 *
	 *	Dy	A2M4	A2M3	A2M2	Rate
	 *	X	1		1		1		Once per minute (at seconds = 00)
	 *	X	1		1		0		Alarm when minutes match
	 *	X	1		0		0		Alarm when hours and minutes match
	 *	0	0		0		0		Alarm when date, hour, min match
	 *	1	0		0		0		Alarm when DoW, hour, min match
	 */

	void setA1Time(uint8_t A1Day, uint8_t A1Hour, uint8_t A1Minute, uint8_t A1Second, 
					uint8_t AlarmBits, bool A1Dy, bool A1h12, bool A1PM); 
		// Set the details for Alarm 1
	void turnOnAlarm(uint8_t Alarm); 
		// Enables alarm 1 or 2 and the external interrupt pin.
		// If Alarm != 1, it assumes Alarm == 2.
	void turnOffAlarm(uint8_t Alarm); 
		// Disables alarm 1 or 2 (default is 2 if Alarm != 1);
		// and leaves the interrupt pin alone.
	bool checkAlarmEnabled(uint8_t Alarm); 
		// Returns T/F to indicate whether the requested alarm is
		// enabled. Defaults to 2 if Alarm != 1.
	bool checkIfAlarm(uint8_t Alarm); 
		// Checks whether the indicated alarm (1 or 2, 2 default);
		// has been activated.

/*=========================================================================*/

/*=========================================================================
    Oscillator functions.
    -----------------------------------------------------------------------*/

	void enableOscillator(bool TF, bool battery, uint8_t frequency); 
		// turns oscillator on or off. True is on, false is off.
		// if battery is true, turns on even for battery-only operation,
		// otherwise turns off if Vcc is off.
		// frequency must be 0, 1, 2, or 3.
		// 0 = 1 Hz
		// 1 = 1.024 kHz
		// 2 = 4.096 kHz
		// 3 = 8.192 kHz 
	void enable32kHz(bool TF); 
		// Turns the 32kHz output pin on (true) or off (false).
	bool oscillatorCheck(void);
		// Checks the status of the OSF (Oscillator Stop Flag).
		// If this returns false, then the clock is probably not
		// giving you the correct time.
		// The OSF is cleared by function setSecond().
			
/*=========================================================================*/

/*=========================================================================
    Converting functions.
    -----------------------------------------------------------------------*/

	uint8_t decToBcd(uint8_t val);
		// Convert normal decimal numbers to binary coded decimal
	uint8_t bcdToDec(uint8_t val);
		// Convert binary coded decimal to normal decimal numbers
	
/*=========================================================================*/

/*=========================================================================
    Operation functions.
    -----------------------------------------------------------------------*/	
	
	uint8_t readControlByte(bool which);
		// Read selected control byte: (0); reads 0x0e, (1) reads 0x0f
	void writeControlByte(uint8_t control, bool which);
		// Write the selected control byte.
		// which == false -> 0x0e, true->0x0f.

/*=========================================================================*/
#endif