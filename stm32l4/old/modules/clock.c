#include "stm32l476xx.h"

void System_Clock_Init_200kHz(void) {
    // Turn on the MSI clock
    RCC->CR |= RCC_CR_MSION;
    // Select MSI as the clock source for the System Clock
    // 00: MSI, 01: HSI, 10: HSE, 11: PLL
    RCC->CFGR &= ~RCC_CFGR_SW;
    // Wait until MSI is ready
    while ((RCC->CR & RCC_CR_MSIRDY) == 0);

    // Once MSI is ready, change MSIRANGE, picking 0x10 for 200kHz
    RCC->CR &= ~RCC_CR_MSIRANGE;
    RCC->CR |= RCC_CR_MSIRANGE_1;

    // Set MSIRGSEL bit, making the clock use the MSIRANGE in RCC_CR
    RCC->CR |= RCC_CR_MSIRGSEL;

    // Wait until MSI is ready and enabled
    while ((RCC->CR & RCC_CR_MSIRDY) == 0);
}
