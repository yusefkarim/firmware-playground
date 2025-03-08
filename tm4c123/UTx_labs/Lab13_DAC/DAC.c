// DAC.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Implementation of the 4-bit digital to analog converter
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// Port B bits 3-0 have the 4-bit DAC

#include "DAC.h"
#include "..//tm4c123gh6pm.h"

// **************DAC_Init*********************
// Initialize 4-bit DAC 
// Input: none
// Output: none
void DAC_Init(void)
{
  //Using PortB3-0
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x02;						//Port B, Run Mode Clock
	delay = SYSCTL_RCGC2_R;						//delay
	GPIO_PORTB_AMSEL_R &= ~0x0F;			//Disable analog functions of PB3-0
	GPIO_PORTB_PCTL_R &= ~0x0000FFFF;	//Clear GPIOPCTL for bits 3-0
	GPIO_PORTB_DIR_R |= 0x0F;					//Set PB3-0 output
	GPIO_PORTB_AFSEL_R &= ~0x0F;			//Clear alt functions
	GPIO_PORTB_DR8R_R |= 0x0F; 				//8mA out
	GPIO_PORTB_DEN_R |= 0x0F;					//Digital enable PB3-0	
}


// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(unsigned long data)
{
	GPIO_PORTB_DATA_R = data;		//Set output accordingly
}
