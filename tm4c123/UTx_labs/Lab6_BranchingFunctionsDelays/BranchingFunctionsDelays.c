// BranchingFunctionsDelays.c Lab 6
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to 
// toggle an LED while a button is pressed and 
// turn the LED on when the button is released.  
// This lab will use the hardware already built into the LaunchPad.
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

//Completed by Yusef Karim
//June 11, 2017

// built-in connection: PF0 connected to negative logic momentary switch, SW2
// built-in connection: PF1 connected to red LED
// built-in connection: PF2 connected to blue LED
// built-in connection: PF3 connected to green LED
// built-in connection: PF4 connected to negative logic momentary switch, SW1

#include "TExaS.h"

#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control

//Global variables
unsigned long SW1;	//Store PF4 input value

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
//User-defined function
void PortFInit(void);
void Delay(void);

int main(void)
	{
  TExaS_Init(SW_PIN_PF4, LED_PIN_PF2);  // activate grader and set system clock to 80 MHz
 
	PortFInit();	//Sets up Port F
  EnableInterrupts(); // enable interrupts for the grader
		
	GPIO_PORTF_DATA_R |= 0x04;	//Sets PF2 as initially on

  while(1)
	{
		SW1 = GPIO_PORTF_DATA_R&0x10;	//Reads the value from SW1 switch
		
		//Checks if PF4(SW1) is enabled, PF4 uses negative logic
		if(!SW1)
		{
			GPIO_PORTF_DATA_R ^= 0x04;	//Toggle PF2
			Delay();	//Short Delay
		}
		else
		{
			GPIO_PORTF_DATA_R |= 0x04;	//Keep PF2 on
		}
			
  }
}
	
//Setup Port F
void PortFInit(void)
{
	unsigned long volatile delay;
	SYSCTL_RCGC2_R |= 0x00000020;     // F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_AMSEL_R = 0x00;        // Disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // GPIO clear bit PCTL 
	GPIO_PORTF_DIR_R = 0x04;					// PF4 as input, PF2 as output
	GPIO_PORTF_AFSEL_R = 0x00;				// Clear alternate functions
	GPIO_PORTF_PUR_R = 0x10;					// Enable pull-up resistor on PF4
	GPIO_PORTF_DEN_R = 0x14;					// Enable digital pins PF4 and PF2
}

//Delay a specified amount of time
//Measuring this using oscilloscope gives a delay with a frequency of 5Hz
void Delay(void)
{
	unsigned long i = 1333333;
	
	while(i > 0)
		i -= 1;
}
