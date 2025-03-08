// Lab13.c
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// edX Lab 13 
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// Port B bits 3-0 have the 4-bit DAC
// Port E bits 3-0 have 4 piano keys

#include "..//tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "TExaS.h"

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void delay(unsigned long msec);

unsigned int current_input;
//Reload value for specific freq:
//reload_value= (bus freq/(DAC outputs per peiod)*(desired freq))
//Ex. (80MHz/(32*523) = 4780
#define C 4780		//523 Hz
#define D 4259		//587 Hz
#define E 3794		//659 Hz								
#define G 3189		//784 Hz						

// Real Lab13 
int main(void)
{ 
	// for the real board grader to work 
	// you must connect PD3 to your DAC output
  TExaS_Init(SW_PIN_PE3210, DAC_PIN_PB3210,ScopeOn); // activate grader and set system clock to 80 MHz
	// PortE used for piano keys, PortB used for DAC        
  Sound_Init(); // initialize SysTick timer and DAC
  Piano_Init();
  EnableInterrupts();  // enable after all initialization are done
	
  while(1)
	{		
		// input from keys to select tone
		current_input = Piano_In();
		
		if(current_input == 0x01)
				Sound_Tone(C);
		else if(current_input == 0x02)
				Sound_Tone(D);
		else if(current_input == 0x04)
				Sound_Tone(E);
		else if(current_input == 0x08)
				Sound_Tone(G);
		else
				Sound_Off();
		delay(10);

  }
            
}

// Inputs: Number of msec to delay
// Outputs: None
void delay(unsigned long msec)
{	
  unsigned long count;
	// repeat while there are still delay
  while(msec > 0 ) 
	{  
    count = 16000;    // about 1ms
		// This while loop takes approximately 3 cycles
    while (count > 0)
      count--;
		
    msec--;
  }
}


