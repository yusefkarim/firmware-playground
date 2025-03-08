// Sound.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Use the SysTick timer to request interrupts at a particular period.
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// This routine calls the 4-bit DAC

#include "Sound.h"
#include "DAC.h"
#include "..//tm4c123gh6pm.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

unsigned char Index = 0;        // Index varies from 0 to 31
const unsigned long SineWave[32] ={
	8,9,11,12,13,14,14,15,15,15,14,14,13,12,
	11,9,8,7,5,4,3,2,2,1,1,1,2,2,3,4,5,7};

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Also calls DAC_Init() to initialize DAC
// Input: none
// Output: none
void Sound_Init(void)
{
	//PortB3-0 output
	DAC_Init();
	//Init SysTick
	NVIC_ST_CTRL_R = 0;			//Disable SysTick during setup
	NVIC_ST_RELOAD_R = 0;		//Number of counts to wait
	NVIC_ST_CURRENT_R = 0;	//Any write clears current
	//Set ISR priority 1
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000;
	//NVIC_ST_CTRL_ENABLE(bit0)+NVIC_ST_CTRL_CLK_SRC(bit1)+NVIC_ST_CTRL_INTEN(bit2)
	NVIC_ST_CTRL_R = 0x07;	//Enable Systick with core clock
}

// **************Sound_Tone*********************
// Change Systick periodic interrupts to start sound output
// Input: interrupt period
//           Units of period are 12.5ns
//           Maximum is 2^24-1
//           Minimum is determined by length of ISR
// Output: none
void Sound_Tone(unsigned long period)
{
	// this routine sets the RELOAD and starts SysTick
	EnableInterrupts();
	NVIC_ST_CTRL_R = 0;			//Disable SysTick during setup
	NVIC_ST_RELOAD_R = period-1;// reload value
	NVIC_ST_CTRL_R = 0x07;	//Enable Systick with core clock
}


// **************Sound_Off*********************
// stop outputing to DAC
// Output: none
void Sound_Off(void)
{
	// this routine stops the sound output
	DisableInterrupts();
	DAC_Out(0x00);
}


// Interrupt service routine
// Executed every 12.5ns*(period)
void SysTick_Handler(void)
{
  Index = (Index+1)&0x1F; 		//Index wraps back to 0 at 31
	DAC_Out(SineWave[Index]);		//Outputs one value each interrupt
}
