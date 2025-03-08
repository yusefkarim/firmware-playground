// TuningFork.c Lab 12
// Runs on LM4F120/TM4C123
// Use SysTick interrupts to create a squarewave at 440Hz.  
// There is a positive logic switch connected to PA3, PB3, or PE3.
// There is an output on PA2, PB2, or PE2. The output is 
//   connected to headphones through a 1k resistor.
// The volume-limiting resistor can be any value from 680 to 2000 ohms
// The tone is initially off, when the switch goes from
// not touched to touched, the tone toggles on/off.
//                   |---------|               |---------|     
// Switch   ---------|         |---------------|         |------
//
//                    |-| |-| |-| |-| |-| |-| |-|
// Tone     ----------| |-| |-| |-| |-| |-| |-| |---------------
//
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2015

 Copyright 2016 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

//Completed by Yusef Karim
//July 19, 2017

#include "TExaS.h"
#include "..//tm4c123gh6pm.h"

//Global variables
unsigned long input;					//Reads PA3 input value
unsigned long previous_input = 0;		//Remembers switch value from last ISR
unsigned char next_state = 0;			//Either 0 or 1 depending on input states

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode

//My functions
void Sound_Init(void);
void SysTick_Handler(void);


int main(void)
{
	// activate grader and set system clock to 80 MHz
  TExaS_Init(SW_PIN_PA3, HEADPHONE_PIN_PA2,ScopeOn); 
  Sound_Init();         
  EnableInterrupts();   // enable after all initialization are done
	
  while(1)
	{
    // main program is free to perform other tasks
    // do not use WaitForInterrupt() here, it may cause the TExaS to crash
  }
}

// input from PA3, output from PA2, SysTick interrupts
void Sound_Init(void)
{
	//**** GPIO ****// 
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000001;			//A Clock
	delay = SYSCTL_RCGC2_R;					//delay
	GPIO_PORTA_AMSEL_R &= ~0x0C;			//Disable analog functions
	GPIO_PORTA_PCTL_R &= ~0x0000FF00;		//GPIO PCTL clear bits 3-2 
	GPIO_PORTA_DIR_R &= ~0x08;				//PA3 input
	GPIO_PORTA_PDR_R |= 0x08;				//Pull-down PA3
	GPIO_PORTA_DIR_R |= 0x04;				//PA2 output
	GPIO_PORTA_AFSEL_R &= ~0x0C;			//Clear alt functions
	GPIO_PORTA_DEN_R |= 0x0C;				//Enable digital pin PA3-2
	
	//**** SysTick ****//
	//Clock is set to 80 MHz, each SysTick take 1/80MHz = 12.5 ns
	//Divide: 80MHz/880Hz = 90909, this is the value we need to pass to reload register
	
	NVIC_ST_CTRL_R = 0;							//Disable SysTick during setup
	NVIC_ST_RELOAD_R = 90909 - 1;				//Number of counts to wait
	NVIC_ST_CURRENT_R = 0;						//Any write clears current
	NVIC_SYS_PRI3_R &= 0x00FFFFFF;				//Give SysTick Priority 0
	//Enable Systick with core clock
	//NVIC_ST_CTRL_ENABLE(bit0)+NVIC_ST_CTRL_CLK_SRC(bit1)+NVIC_ST_CTRL_INTEN(bit2)
	NVIC_ST_CTRL_R = 0x07;
	
}

// called at 880 Hz
//This will create our square wave
void SysTick_Handler(void)
{
		//Read value of PA3 button input
		input = GPIO_PORTA_DATA_R&0x08;
		
		//If previous input was off and current input is high
		if((previous_input == 0x00) && (input == 0x08))
			next_state ^= 1;	//Toggle next state
		
		if(next_state == 1)
			GPIO_PORTA_DATA_R ^= 0x04;	//Toggle PA2
		
		previous_input = input;	//Remember current ISR input value for later
		
}
