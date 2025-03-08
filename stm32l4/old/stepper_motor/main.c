// Author: Yusef Karim
#include "stm32l476xx.h"

// Function prototypes
void GPIOC_Init(void);

// Four bits correspond to PC3-0 (least significant bit is PC0)
// Full step:             0b1001, 0b1010, 0b0110, 0b0101
unsigned char FULLSTEP[4] = {0x9, 0xA, 0x6, 0x5};

int main(void)
{
    int i, j, k;
    short PC0, PC1, PC2, PC3;
    GPIOC_Init();
    // 48 steps * 7.5 degrees per step = 360 degrees
    for(i = 0; i < 48; i++) {
        // Cycle through FULLSTEP to get a fullstep
        for(j = 0; j < 4; j++) {
            for(k = 0; k < 6000; k++) {;}   // Allow for a short delay between steps
            // Determine the current steps bits for each output PC0-3
            PC3 = (FULLSTEP[j] & 0x8) >> 3;
            PC2 = (FULLSTEP[j] & 0x4) >> 2;
            PC1 = (FULLSTEP[j] & 0x2) >> 1;
            PC0 = FULLSTEP[j] & 0x1;
            // If output should be 1, set the GPIO bit else clear it
            GPIOC->ODR = (PC3) ? (GPIOC->ODR | 0x8) : (GPIOC->ODR & ~0x8);
            GPIOC->ODR = (PC2) ? (GPIOC->ODR | 0x4) : (GPIOC->ODR & ~0x4);
            GPIOC->ODR = (PC1) ? (GPIOC->ODR | 0x2) : (GPIOC->ODR & ~0x2);
            GPIOC->ODR = (PC0) ? (GPIOC->ODR | 0x1) : (GPIOC->ODR & ~0x1);
        }
    }
    while(1);
}

void GPIOC_Init(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;                // Enable clock for port C
    GPIOC->MODER &= ~(3UL | 3UL<<2 | 3UL<<4 | 3UL<<6);  // Clear mode bits for PC0-3
    GPIOC->MODER |= (1UL | 1UL<<2 | 1UL<<4 | 1UL<<6);   // Set PC0-3 mode as output
    GPIOC->OTYPER &= ~(1UL | 1UL<<1 | 1UL<<2 | 1UL<<3); // Set PC0-3 output type, push-pull
    GPIOC->OSPEEDR &= ~(3UL | 3UL<<2 | 3UL<<4 | 3UL<<6);// Clear speed bits (low speed)
    GPIOC->PUPDR &= ~(3UL | 3UL<<2 | 3UL<<4 | 3UL<<6);  // Set PC0-3 as no pull-up/down
}
