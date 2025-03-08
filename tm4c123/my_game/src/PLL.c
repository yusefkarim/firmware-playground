#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "PLL.h"

//------------PLL_Init------------//
//Gets 80MHz clock from PLL
//Using the 400MHz PLL we get the bus frequency by dividing
//400MHz / (SYSDIV2+1). Therefore, 400MHz/(4+1) = 80 MHz bus frequency
#define SYSDIV2 4
void PLL_Init(void)
{
    SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2;   //Use RCC2 for advanced features
    SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2;   //Bypass PLL during initialization
    SYSCTL_RCC_R &= ~SYSCTL_RCC_XTAL_M;     //Clear XTAL field
    SYSCTL_RCC_R += SYSCTL_RCC_XTAL_16MHZ;  //Use 16Mhz crystal
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_OSCSRC2_M;//Clear oscillator source field
    SYSCTL_RCC2_R += SYSCTL_RCC2_OSCSRC2_MO;//Use main oscillator source field
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2;   //Activate PLL by clearing PWRDN

    //As explained above we use 400MHz PLL and specified SYSDIV2 for 80MHz clock
    SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;    //Use 400MHz PLL
    //Clear system clock divider field then configure 80Mhz clock
    SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~0x1FC00000) + (SYSDIV2<<22);
    //Wait for the PLL to lock by polling PLLLRIS
    while((SYSCTL_RIS_R&SYSCTL_RIS_PLLLRIS) == 0){};
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2;  //Enable PLL by clearing BYPASS
}

