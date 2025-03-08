// Author: Yusef Karim
#include "stm32l476xx.h"


/* Enables(enable = 1) or disables(enable=0) a peripheral interrupt. Does
* this by calculating the index of the interrupt register array and then
* setting the bit in the array using the passed in IRQ_number.
*
* Args:
*      IRQ_number: interrupt number (number between 0 and 240)
*      enable: If enable=1, enables interrupt in NVIC else disables the interrupt
*/
void Peripheral_Interrupt_Enable(int IRQ_number, int priority, unsigned short enable) {
    // WordOffset = IRQ_number >> 5  (same as IRQ_number/32)
    // BitOffset = IRQ_number & 0x1F (same as IRQ_number mod 32)
    if(enable) {
        NVIC->ISER[IRQ_number >> 5] = 1 << (IRQ_number & 0x1F);     // Enable interrupt
        NVIC->IP[IRQ_number] = (priority << 4) & 0xFF;              // Set priority
    }
    else
        NVIC->ICER[IRQ_number >> 5] = 1 << (IRQ_number & 0x1F);     // Disable interrupt
}


/*
 * Initializes SysTick with interrupt enabled. Uses the processor clock (default 4MHz).
 * Overwrite the SysTick_Handler function defined in startup_stm32l476xx.s to do
 * tasks when the SysTick counts down to 0.
 *
 * Args:
 *      ticks: The number of clock ticks you want to wait before the SysTick interrupt
 *             occurs. Ex. Passing 4000000 for ticks will make the interrupt trigger
 *             every 1 second if the processor clock is set to 4MHz.
 */
void SysTick_Init(uint32_t ticks) {
    // Disable SysTick IRQ and SysTick counter
    SysTick->CTRL = 0;
    // Set reload register
    SysTick->LOAD = ticks - 1;

    // Set interrupt priority of SysTick as least urgent (highest priority number)
    // This function is defined in core_cm4.h
    NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);

    // Reset the SysTick counter value (by writing to it)
    SysTick->VAL = 0;

    // Select processor clock
    // 1 = processor clock, 0 = external clock
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

    // Enables SysTick exception request
    // 1 = counting down to zero asserts the SysTick exception request
    // 0 = counting down to zero does not assert the SysTick exception request
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

    // Enable SysTick timer
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

