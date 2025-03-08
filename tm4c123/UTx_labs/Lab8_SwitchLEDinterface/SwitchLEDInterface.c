// ***** 0. Documentation Section *****
// SwitchLEDInterface.c for Lab 8
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to toggle an LED
// while a button is pressed and turn the LED on when the
// button is released.  This lab requires external hardware
// to be wired to the LaunchPad using the prototyping board.
// January 15, 2016
//      Jon Valvano and Ramesh Yerraballi

//Completed by Yusef Karim
//June 14, 2017

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****
unsigned long PE0;		//PE0 acts as an input to the breadboard switch

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

// ***** 3. Subroutines Section *****
void PortEInit(void);
void Delayms(unsigned long msec);
unsigned int SwitchPressed(void);

// PE0 connected to positive logic momentary switch using 10k ohm pull down resistor
// PE1 connected to positive logic LED through 470 ohm current limiting resistor
// To avoid damaging your hardware, ensure that your circuits match the schematic
// shown in Lab8_artist.sch (PCB Artist schematic file) or 
// Lab8_artist.pdf (compatible with many various readers like Adobe Acrobat).
int main(void){ 
//**********************************************************************
// The following version tests input on PE0 and output on PE1
//**********************************************************************
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1, ScopeOn);  // activate grader and set system clock to 80 MHz
  
	PortEInit();
  EnableInterrupts();           // enable interrupts for the grader
	
	GPIO_PORTE_DATA_R |= 0x02;		//Set PE1 on
	Delayms(100);									//Give initial delay of 100 ms
  while(1)
	{
		//Check if switch has been pressed
		if(SwitchPressed() == 1)
		{
			GPIO_PORTE_DATA_R ^= 0x02;	//Toggle PE1
			Delayms(100);								//Delay 100 ms after each toggle
		}
  }
  
}

//Initialize Port E
void PortEInit(void)
{
	unsigned long volatile delay;
	SYSCTL_RCGC2_R |=	0x00000010;		//E Clock
	delay = SYSCTL_RCGC2_R;					//Delay
	GPIO_PORTE_AMSEL_R = 0x00;			//Disable analog function
	GPIO_PORTE_PCTL_R = 0x00000000;	//Clear bit PCTL
	GPIO_PORTE_DIR_R = 0x02;				//PE0 as input, PE1 as output
	GPIO_PORTE_AFSEL_R = 0x00;			//Clear alternate functions
	GPIO_PORTE_DEN_R = 0x03;				//Enable PE0 and PE1
}

//Delay user-specified amount of milliseconds
void Delayms(unsigned long msec)
{
	unsigned long i;
	while(msec > 0)
	{
		//Determined using oscilloscope, aprox. 1ms to go to 0
		i = 400000 / 30;
		while(i > 0)
			i--;
		msec--;
	}
}

//Returns 1 if PE0 has been pressed, else 0
unsigned int SwitchPressed(void)
{
	PE0 = GPIO_PORTE_DATA_R&0x01;		//Reads value from PE0
	
	if(PE0)
		return 1;
	else
		return 0;
}
