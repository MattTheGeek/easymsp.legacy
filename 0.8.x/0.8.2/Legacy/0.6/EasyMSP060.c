/* Licence Information
Fair Use Licence
======================================================
Copyright Matthew L. Burmeister Mar. 1th 2011 All Rights Reserved.

Usage of the works is permitted provided that this instrument is retained with the works, so that any entity that uses the works is notified of this instrument.

DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
*/

/*Programmer Comments
This licence may be changed at product release and/or in other devlopmental releases.
This file is the standard EasyMSP library version 0.6 Alpha

EasyMSP has gone from idea into a project in less than 14 weeks, Truly ammazing i belive. 
I Hope you enjoy this pre-dev-alpha release of EasyMSP. also i've created a few sites for you to check out.

www.code.google.com/p/easymsp/  <- Holds the source
and
www.sites.google.com/site/easymsp/ <- Main site

Again, i hope you find this release to be useful

Regards,
Matthew L. Burmeister
Owner and Head Dev.
*/

//#include <msp430g2231.h>
//Alright, lets define the pins
//We use a bit-mask to change the states on the pins
const int M16 = 0x01;
const int M12 = 0x02;
const int M8 = 0x03;
const int M1 = 0x04;
const int K500 = 0x05;
const int K100 = 0x06;
const int K12 = 0x07;

const int pin0=0x01;
const int pin1=0x02;
const int pin2=0x04;
const int pin3=0x08;
const int pin4=0x10;
const int pin5=0x20;
const int pin6=0x40;
const int pin7=0x80;
const int TEMP=0xFF;

const int YES = 1;
const int NO = 0;
const int MSBFIRST = 0;
const int LSBFIRST = 1;
const int FAST = 1;
const int SLOW = 0;

const int INPUT = 0;
const int OUTPUT = 1;

const int DEFAULT = 1;
const int INTERNAL = 2;
const int INTERNAL1V1 = 3;
const int INTERNAL2V56 = 4;
const int EXTERNAL = 5;

//EasyMSP Prototypes
//=====================================

//GPIO
int read(int pin);
void high(int pin);
void low(int pin);

//System
void stopwd();
void delaySec(int time);
void delayMs(int time);
void setup();
void freqSet(int freq);
void reset();
void shutdown();

//SPI
void spiOut(char spidata);
char spiIn(int timeout);

//Logic
void bitWrite(int, char, char);
char bitRead(int, char);
void bitSet(int, char);
void bitClear(int, char);
char bit(char);

//I2C
void i2cOut(int Address, char Data);
char i2cIn(int address, char Data);

//Timer
void pwmOut(char pin, int freq, int duty);
void pulseOut(char pin, int duration);
int pulseIn(char pin, duration);

//Arduino Prototypes
//======================================
void digitalWrite(int pin, int mode);
int digitalRead(int pin);
void pinMode(int pin, int state);
int analogRead(int pin);
void analogReference(int mode);

/* Begin Functions */

//Test Main()


//high()
//=============================================
//Inputs: Pin to make high
//Returns: Nothing
//Actions: Causes a pin to go logical high
void high(int pin)
{
  P1DIR |= pin;
  P1OUT |= pin;
}

//Low()
//=============================================
//Inputs: Pin to make low
//Returns: Nothing
//Actions: Causes a pin to go logical low
void low(int pin)
{
  P1DIR |= pin;
  P1OUT &= ~pin; 
}

//read()
//=============================================
//Inputs: pin to read
//Returns: pin state
//Actions: ANDs the P*IN Register and returns a boolean value depending on the state
int read(int pin)
{
	int result;
	result = pin & P1IN;
	
	if (result > 0)
	{
		return 1;
	}
	
	else
	{
		return 0;
	}  
}

//This function mimics the arduino's digitalWrite function
void digitalWrite(int pin, int state)
{
	if (state == 0)
	{
		low(pin);
	}
	else
	{
		high(pin);
	}
}

//this function mimics the arduinos pinMode command
void pinMode(int pin, int State)
{
	if(State == 0)
	{
		P1DIR &= ~pin; 
	}
	else
	{
		P1DIR |= pin;
	}
}

//This function mimics the arduinos digitalRead command
int digitalRead(int pin)
{
	int var;
	var = read(pin);
	return (var);
}

//delayMS(time)
//=============================================
//Inputs: Delay time.
//Returns: Nothing
//Actions: Delays the MCU for said mS.
void delaySec(int time)
{
	while(time > 0)
	{
		
		delayMs(1000);
		time = time - 1;
	}
}

//delayMS(time)
//=============================================
//Inputs: Delay time.
//Returns: Nothing
//Actions: Delays the MCU for said mS.
void delayMs(int time)
{
	while(time != 0)
	{
		
		int a=16;
	
		while (a!= 0)
			{
				a=a-1;
			}
		time = time - 1;
	}
		
}

//spiOut
//=============================================
//Inputs: Data to be sent
//Returns: Nothing
void spiOut(char spidata)
{
	
	
	
	USISRL = spidata;
	USICNT = 8;
}

int analogRead(int pin)
{
	//Ok, we need to setup the ADC10.
	//ADC10CTL0 |= SREF_0; //Set Ref to VCC and VDD
	ADC10CTL0 |= ADC10SHT_0; //Sample And Hold 4 clokcs
	//ADC10CTL0 |= ADC10SR; // enable for low power mode
	//ADC10CTL0 |= REFBURST;
	switch(pin)
	{
		case 0x01:
		ADC10CTL1 |= INCH_0;
		break;
		
		case 0x02:
		ADC10CTL1 |= INCH_1;
		break;
		
		case 0x04:
		ADC10CTL1 |= INCH_2;
		break;
		
		case 0x08:
		ADC10CTL1 |= INCH_3;
		break;
		
		case 0x010:
		ADC10CTL1 |= INCH_4;
		break;
		
		case 0x20:
		ADC10CTL1 |= INCH_5;
		break;
		
		case 0x40:
		ADC10CTL1 |= INCH_6;
		break;
		
		case 0x80:
		ADC10CTL1 |= INCH_7;
		break;
		
		case 0xFF:
		ADC10CTL1 |= INCH_10;
		break;
	}
	
	//ADC10CTL1 |= ISSH;
	ADC10CTL0 |= ADC10ON + ENC;
	ADC10CTL0 |= ADC10SC;
	
	int ADCWAIT=4;
	
	while (ADCWAIT != 0)
	{
		ADCWAIT = ADCWAIT - 1;
	}
	
	return (ADC10MEM);
	
}


void analogReference(int ref)
{
	if (ref == DEFAULT)
	{
		ADC10CTL0 &= ~SREF_7; //Clear the register
		ADC10CTL0 |= SREF_0;
	}
	
	if (ref == INTERNAL)
	{
		ADC10CTL0 &= ~SREF_7; //Clear the register
		ADC10CTL0 |= SREF_0;
	}
	
	if (ref == INTERNAL1V1)
	{
		ADC10CTL0 &= ~SREF_7; //Clear the register
		ADC10CTL0 |= SREF_1 + REFON;
		ADC10CTL0 &= ~REF2_5V;
	}
	
	if (ref == INTERNAL2V56)
	{
		ADC10CTL0 &= ~SREF_7; //Clear the register
		ADC10CTL0 |= SREF_1 + REFON + REF2_5V;
	}
	
	if (ref == EXTERNAL)
	{
		ADC10CTL0 &= ~SREF_7; //Clear the register
		ADC10CTL0 |= SREF_2;
	}
}


//Reset the controller
void reset()
{
	WDTCTL = 0;
}

void shutdown()
{
	//Enters the CPU into LPM4
}

//stopwd()
//=============================================
//Inputs: Nothing
//Returns: Nothing
//Actions: Stops the system watchdog.
void stopwd()
{
   //Stop the watchdog
   WDTCTL = WDTPW + WDTHOLD;
}

//setup()
//=============================================
//Inputs: Nothing
//Returns: Nothing
//Actions: Setups the EasyMSP System.
void setup()
{
  	stopwd();
  //Other tasks required here
}

//Changes the systems clock
//Unfunctional Prototype
void setfreq(int FREQ)
{
	
		if (FREQ == M16)
		{
		//Set CPU to 16MHZ
		
		}
		
		if (FREQ == M12)
		{
		//Set CPU to 12Mhz
		
		}
		
		if (FREQ == M8)
		{
		//Set CPU to 4Mhz
		
		}
		
		if (FREQ == M1)
		{
		//Set CPU to 1MHZ
		BCSCTL1 = CALBC1_1MHZ;
		DCOCTL = CALDCO_1MHZ;
		}	
			
		if (FREQ == K500)
		{
				//Set CPU to 500KHZ
		}
		
		if (FREQ == K100)
		{
		//Set CPU to 100KHZ
		}
		
		if (FREQ == K12)
		{
		//Set CPU to 12Khz
		}
}

void bitSet(int number, char bit)
{
	number |= bit;
}

void bitClear(int number, char bit)
{
	number &= ~bit;
}

void bitWrite(int number,char bit,char val)
{
	if (val == 1)
	{
		bitSet(number, bit);
	}
	else
	{
		bitClear(number, bit);
	}
	
}

char bitRead(int number,char  bit)
{
	char val;
	val = number & bit;
	return (val);
}

char bit(char number)
{
	char val;
	switch (number)
	{
		case 0:
		val = 0x01;
		break;
		
		case 1:
		val = 0x02;
		break;
		
		case 2:
		val = 0x04;
		break;
		
		case 3:
		val = 0x08;
		break;
		
		case 4:
		val = 0x10;
		break;
		
		case 5:
		val = 0x20;
		break;
		
		case 6:
		val = 0x40;
		break;
		
		case 7:
		val = 0x80;
		break;
	}
	return (val);
}
