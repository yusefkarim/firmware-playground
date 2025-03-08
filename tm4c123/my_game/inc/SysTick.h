#ifndef SYSTICK_H
#define SYSTICK_H

//------------SysTick_Init------------//
//Enables SysTick with passed in reload value
//Enables interrupts with Priority 0 for SysTick
void SysTick_Init(unsigned long reload);

//------------SysTick_Wait1ms------------//
//Uses SysTick to count down 1ms*(passed value)
void SysTick_Wait1ms(unsigned long ms);

#endif
