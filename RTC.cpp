/*
 *
 *  Description:
 *  -------------
 *
 *    project name: RTC 
 *    File Name: RTC.cpp
 *
 *
 *    Version:		Date:        Author:         comment:
 *    --------		-----------  --------------  -------------------
 *    1.00			07.10.2017   Kevin Perillo   build.
 *
 *
 *    Copyright (c) MootSeeker 2017
 ***********************************************************************************/

#include "RTC.h"


/*=========================================================================
    Get Seconds from the Register.
  -----------------------------------------------------------------------*/
uint8_t getSecond(void) 
{
	Wire.beginTransmission(Clock_ADDRESS);	// Begin Transmission with the Clock (Address = 0x68).
	Wire.write(DS3231_REGISTER_Second);		// Write Adress of the Register. 
	Wire.endTransmission();					// End Transmission. 

	Wire.requestFrom(Clock_ADDRESS, 1);		// Send Stop-Bit
	return bcdToDec(Wire.read());			// Read Register Value and convert to DEC. 
}

/*=========================================================================
    Get Minutes from the Register.
  -----------------------------------------------------------------------*/
uint8_t getMinute(void) 
{
	Wire.beginTransmission(Clock_ADDRESS);
	Wire.write(DS3231_REGISTER_Minute);
	Wire.endTransmission();

	Wire.requestFrom(Clock_ADDRESS, 1);
	return bcdToDec(Wire.read());
}

/*=========================================================================
    Get Hours from the Register.
  -----------------------------------------------------------------------*/
uint8_t getHour(bool& h12, bool& PM)		// Set 12/24h mode  | Activate AM/PM. 
{
	uint8_t temp_buffer;
	uint8_t hour;
	
	Wire.beginTransmission(Clock_ADDRESS);
	Wire.write(DS3231_REGISTER_Hour);
	Wire.endTransmission();

	Wire.requestFrom(Clock_ADDRESS, 1);
	temp_buffer = Wire.read();
	
	h12 = (temp_buffer & 0x40);
	if (h12) 
	{
		PM = (temp_buffer & 0x20);
		hour = bcdToDec(temp_buffer & 0x1F);
	} 
	else 
	{
		hour = bcdToDec(temp_buffer & 0b3F);
	}
	return hour;
}

/*=========================================================================
    Get Day of Week from the Register.
  -----------------------------------------------------------------------*/
uint8_t getDoW(void) 
{
	Wire.beginTransmission(Clock_ADDRESS);
	Wire.write(DS3231_REGISTER_DoW);
	Wire.endTransmission();

	Wire.requestFrom(Clock_ADDRESS, 1);
	return bcdToDec(Wire.read());
}

/*=========================================================================
    Get Date (Day) from the Register.
  -----------------------------------------------------------------------*/
uint8_t getDate(void) 
{
	Wire.beginTransmission(Clock_ADDRESS);
	Wire.write(DS3231_REGISTER_Date);
	Wire.endTransmission();

	Wire.requestFrom(Clock_ADDRESS, 1);
	return bcdToDec(Wire.read());
}

/*=========================================================================
    Get Month from the Register.
  -----------------------------------------------------------------------*/
uint8_t getMonth(bool& Century) 
{
	uint8_t temp_buffer;
	uint8_t hour;
	Wire.beginTransmission(Clock_ADDRESS);
	Wire.write(DS3231_REGISTER_Month);
	Wire.endTransmission();

	Wire.requestFrom(Clock_ADDRESS, 1);
	temp_buffer = Wire.read();
	
	Century = temp_buffer & 0x80;
	return (bcdToDec(temp_buffer & 0xEF));
}

/*=========================================================================
    Get Year from the Register.
  -----------------------------------------------------------------------*/
uint8_t getYear(void) 
{
	Wire.beginTransmission(Clock_ADDRESS);
	Wire.write(DS3231_REGISTER_Year);
	Wire.endTransmission();

	Wire.requestFrom(Clock_ADDRESS, 1);
	return bcdToDec(Wire.read());
}

/*=========================================================================
    Set Second in Register.
  -----------------------------------------------------------------------*/
void setSecond(uint8_t Second) 
{
	// Sets the seconds 
	// This function also resets the Oscillator Stop Flag, which is set
	// whenever power is interrupted.
	Wire.beginTransmission(Clock_ADDRESS);
	Wire.write(DS3231_REGISTER_Second);
	Wire.write(decToBcd(Second));	
	Wire.endTransmission();
	
	// Clear OSF flag
	uint8_t temp_buffer = readControlByte(1);
	writeControlByte((temp_buffer & 0xEF), 1);
}

/*=========================================================================
    Set Minutes in Register.
  -----------------------------------------------------------------------*/
void setMinute(uint8_t Minute) 
{
	// Sets the minutes 
	Wire.beginTransmission(Clock_ADDRESS);
	Wire.write(DS3231_REGISTER_Minute);
	Wire.write(decToBcd(Minute));	
	Wire.endTransmission();
}

/*=========================================================================
    Set Hour in Register.
  -----------------------------------------------------------------------*/
void setHour(uint8_t Hour) 
{
	// Sets the hour, without changing 12/24h mode.
	// The hour must be in 24h format.

	bool h12;

	// Start by figuring out what the 12/24 mode is
	Wire.beginTransmission(Clock_ADDRESS);
	Wire.write(DS3231_REGISTER_Hour);
	Wire.endTransmission();
	Wire.requestFrom(Clock_ADDRESS, 1);
	
	h12 = (Wire.read() & 0b01000000);
	// if h12 is true, it's 12h mode; false is 24h.

	if (h12) {
		// 12 hour
		if (Hour > 12) {
			Hour = decToBcd(Hour-12) | 0b01100000;
		} else {
			Hour = decToBcd(Hour) & 0b11011111;
		}
	} else {
		// 24 hour
		Hour = decToBcd(Hour) & 0b10111111;
	}

	Wire.beginTransmission(Clock_ADDRESS);
	Wire.write(DS3231_REGISTER_Hour);
	Wire.write(Hour);
	Wire.endTransmission();
}

/*=========================================================================
    Set Day of Week in Register.
  -----------------------------------------------------------------------*/
void setDoW(uint8_t DoW) 
{
	// Sets the Day of Week
	Wire.beginTransmission(Clock_ADDRESS);
	Wire.write(DS3231_REGISTER_DoW);
	Wire.write(decToBcd(DoW));	
	Wire.endTransmission();
}

/*=========================================================================
    Set Date (Day) in Register.
  -----------------------------------------------------------------------*/
void setDate(uint8_t Date) 
{
	// Sets the Date
	Wire.beginTransmission(Clock_ADDRESS);
	Wire.write(DS3231_REGISTER_Date);
	Wire.write(decToBcd(Date));	
	Wire.endTransmission();
}

/*=========================================================================
    Set Month in Register.
  -----------------------------------------------------------------------*/
void setMonth(uint8_t Month) 
{
	// Sets the month
	Wire.beginTransmission(Clock_ADDRESS);
	Wire.write(DS3231_REGISTER_Month);
	Wire.write(decToBcd(Month));	
	Wire.endTransmission();
}

/*=========================================================================
    Set Year in Register.
  -----------------------------------------------------------------------*/
void setYear(uint8_t Year) 
{
	// Sets the year
	Wire.beginTransmission(Clock_ADDRESS);
	Wire.write(DS3231_REGISTER_Year);
	Wire.write(decToBcd(Year));	
	Wire.endTransmission();
}

/*=========================================================================
    Set Clock Mode in Register.
  -----------------------------------------------------------------------*/
void setClockMode(bool h12) 
{
	// sets the mode to 12-hour (true) or 24-hour (false).
	// One thing that bothers me about how I've written this is that
	// if the read and right happen at the right hourly millisecnd,
	// the clock will be set back an hour. Not sure how to do it better, 
	// though, and as long as one doesn't set the mode frequently it's
	// a very minimal risk. 
	// It's zero risk if you call this BEFORE setting the hour, since
	// the setHour() function doesn't change this mode.
	
	uint8_t temp_buffer;

	// Start by reading byte 0x02.
	Wire.beginTransmission(Clock_ADDRESS);
	Wire.write(DS3231_REGISTER_Hour);
	Wire.endTransmission();
	Wire.requestFrom(Clock_ADDRESS, 1);
	temp_buffer = Wire.read();

	// Set the flag to the requested value:
	if (h12) 
	{
		temp_buffer = temp_buffer | 0x01000000;
	} 
	else 
	{
		temp_buffer = temp_buffer & 0x10111111;
	}

	// Write the byte
	Wire.beginTransmission(Clock_ADDRESS);
	Wire.write(DS3231_REGISTER_Hour);
	Wire.write(temp_buffer);
	Wire.endTransmission();
}

/*=========================================================================
    Get Temperature in Register.
  -----------------------------------------------------------------------*/
float getTemperature() 
{
	// Checks the internal thermometer on the DS3231 and returns the 
	// temperature as a floating-point value.
  
  uint8_t tMSB, tLSB;
  float temp3231;
  
  // temp registers (11h-12h) get updated automatically every 64s
  Wire.beginTransmission(Clock_ADDRESS);
  Wire.write(DS3231_REGISTER_MSB_Temp);
  Wire.endTransmission();
  Wire.requestFrom(Clock_ADDRESS, 2);

  // Should I do more "if available" checks here?
  if(Wire.available()) 
  {
    tMSB = Wire.read(); //2's complement int portion
    tLSB = Wire.read(); //fraction portion

    temp3231 = ((((short)tMSB << 8) | (short)tLSB) >> 6) / 4.0;
  }
  else 
  {
    temp3231 = -9999; // Some obvious error value
  }  
  return temp3231;
}

/*=========================================================================
    Enable Oscillator in Register.
  -----------------------------------------------------------------------*/
void enableOscillator(bool TF, bool battery, uint8_t frequency) 
{
	// turns oscillator on or off. True is on, false is off.
	// if battery is true, turns on even for battery-only operation,
	// otherwise turns off if Vcc is off.
	// frequency must be 0, 1, 2, or 3.
	// 0 = 1 Hz
	// 1 = 1.024 kHz
	// 2 = 4.096 kHz
	// 3 = 8.192 kHz (Default if frequency byte is out of range)
	if (frequency > 3) frequency = 3;
	// read control byte in, but zero out current state of RS2 and RS1.
	uint8_t temp_buffer = readControlByte(0) & 0b11100111;
	if (battery) 
	{
		// turn on BBSQW flag
		temp_buffer = temp_buffer | 0b01000000;
	} 
	else 
	{
		// turn off BBSQW flag
		temp_buffer = temp_buffer & 0b10111111;
	}
	if (TF) 
	{
		// set ~EOSC to 0 and INTCN to zero.
		temp_buffer = temp_buffer & 0b01111011;
	} 
	else 
	{
		// set ~EOSC to 1, leave INTCN as is.
		temp_buffer = temp_buffer | 0b10000000;
	}
	// shift frequency into bits 3 and 4 and set.
	frequency = frequency << 3;
	temp_buffer = temp_buffer | frequency;
	// And write the control bits
	writeControlByte(temp_buffer, 0);
}

void enable32kHz(bool TF) 
{
	// turn 32kHz pin on or off
	uint8_t temp_buffer = readControlByte(1);
	if (TF) 
	{
		// turn on 32kHz pin
		temp_buffer = temp_buffer | 0b00001000;
	} 
	else 
	{
		// turn off 32kHz pin
		temp_buffer = temp_buffer & 0b11110111;
	}
	writeControlByte(temp_buffer, 1);
}

bool oscillatorCheck() 
{
	// Returns false if the oscillator has been off for some reason.
	// If this is the case, the time is probably not correct.
	uint8_t temp_buffer = readControlByte(1);
	bool result = true;
	if (temp_buffer & 0b10000000) 
	{
		// Oscillator Stop Flag (OSF) is set, so return false.
		result = false;
	}
	return result;
}


uint8_t decToBcd(uint8_t val) 
{
// Convert normal decimal numbers to binary coded decimal
	return ((val/10*16) + (val%10));
}

uint8_t bcdToDec(uint8_t val) 
{
// Convert binary coded decimal to normal decimal numbers
	return ((val/16*10) + (val%16));
}

uint8_t readControlByte(bool which) 
{
	// Read selected control byte
	// first byte (0) is 0x0e, second (1) is 0x0f
	Wire.beginTransmission(Clock_ADDRESS);
	if (which) 
	{
		// second control byte
		Wire.write(0x0f);
	} 
	else 
	{
		// first control byte
		Wire.write(0x0e);
	}
	Wire.endTransmission();
	Wire.requestFrom(Clock_ADDRESS, 1);
	return Wire.read();	
}

void writeControlByte(uint8_t control, bool which) 
{
	// Write the selected control byte.
	// which=false -> 0x0e, true->0x0f.
	Wire.beginTransmission(Clock_ADDRESS);
	if (which) 
	{
		Wire.write(0x0f);
	} 
	else 
	{
		Wire.write(0x0e);
	}
	
	Wire.write(control);
	Wire.endTransmission();
}