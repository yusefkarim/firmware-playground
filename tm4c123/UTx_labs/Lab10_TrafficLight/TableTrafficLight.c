// ***** 0. Documentation Section *****
// TableTrafficLight.c for Lab 10
// Runs on LM4F120/TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.  
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

// east/west red light connected to PB5
// east/west yellow light connected to PB4
// east/west green light connected to PB3
// north/south facing red light connected to PB2
// north/south facing yellow light connected to PB1
// north/south facing green light connected to PB0
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)

//Completed by Yusef Karim
//June 24, 2017
/***WARNING: This lab is wild***/

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

// ***** 3. Subroutines Section *****
//void PLL_Init(void);
void SysTick_Init(void);
void PortBEF_Init(void);
void SysTick_Wait1ms(unsigned long ms);

//The three inputs PE2-PE0 from PortE
#define SENSOR (*((volatile unsigned long *)0x4002401C))
	
//Linked Data Structure, FSM
typedef struct StateMachine_struct
{
	unsigned long PBOut;		//PB5-0 for the traffic lights
	unsigned long PFOut;		//Onboard PF3 & PF1 for pedestrian lights
	unsigned long Time;			//Delay in 1ms units
	unsigned long Next[8];	//Next state depends on current state of inputs
}StateMachine;

#define GoW 0
#define WaitW 1
#define GoS 2
#define WaitS 3
#define GoP 4
#define NoP1 5
#define POff1 6
#define NoP2 7
#define POff2 8

StateMachine TrafficSystem[9] ={
	{0x0C, 0x02, 500, {GoW, GoW, WaitW, WaitW, WaitW, WaitW, WaitW, WaitW}},
	{0x14, 0x02, 500,	{GoS, GoS, GoS,	GoS, GoP,	GoP, GoP,	GoS}},
	{0x21, 0x02, 500,	{GoS, WaitS, GoS,	WaitS, WaitS,	WaitS, WaitS,	WaitS}},
	{0x22, 0x02, 500,	{GoW, GoW, GoW, GoW, GoP,	GoP, GoP,	GoP}},
	{0x24, 0x08, 500,	{NoP1, NoP1, NoP1, NoP1, GoP,	GoP, GoP,	NoP1}},
	{0x24, 0x02, 500,	{POff1,	POff1, POff1,	POff1, POff1,	POff1, POff1,	POff1}},
	{0x24, 0x00, 500,	{NoP2, NoP2, NoP2, NoP2, NoP2, NoP2, NoP2, NoP2}},
	{0x24, 0x02, 500,	{POff2, POff2, POff2, POff2, POff2, POff2, POff2, POff2}},
	{0x24, 0x00, 500,	{GoW, GoW, GoS,	GoW, GoP, GoP, GoP,	GoW}}	
};
unsigned long State , Input; //State is index to current state


int main(void)
{ 
  TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210,ScopeOff); // activate grader and set system clock to 80 MHz
  EnableInterrupts();
	
	//PLL_Init();
	SysTick_Init();
	PortBEF_Init();
	
	State = GoW;
	
  while(1)
	{
		GPIO_PORTB_DATA_R = TrafficSystem[State].PBOut;
		GPIO_PORTF_DATA_R = TrafficSystem[State].PFOut;
		SysTick_Wait1ms(TrafficSystem[State].Time);
		Input = SENSOR;
		State = TrafficSystem[State].Next[Input];
  }
}

// ***** Function definitions *****

//Woopsies, already activated at 80Mhz by the Texas grader.
/*
//Using the 400Mhz PLL we get the bus frequency by dividing
//400Mhz / (SYSDIV2+1). Therefore, 400Mhz/(4+1) = 80 Mhz bus frequency
#define SYSDIV2 4
//Gets clock from PLL, copied from the available course source files
void PLL_Init(void)
{
	  // 0) configure the system to use RCC2 for advanced features
  //    such as 400 MHz PLL and non-integer System Clock Divisor
  SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2;
  // 1) bypass PLL while initializing
  SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2;
  // 2) select the crystal value and oscillator source
  SYSCTL_RCC_R &= ~SYSCTL_RCC_XTAL_M;   // clear XTAL field
  SYSCTL_RCC_R += SYSCTL_RCC_XTAL_16MHZ;// configure for 16 MHz crystal
  SYSCTL_RCC2_R &= ~SYSCTL_RCC2_OSCSRC2_M;// clear oscillator source field
  SYSCTL_RCC2_R += SYSCTL_RCC2_OSCSRC2_MO;// configure for main oscillator source
  // 3) activate PLL by clearing PWRDN
  SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2;
  // 4) set the desired system divider and the system divider least significant bit
  SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;  // use 400 MHz PLL
  SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~0x1FC00000) // clear system clock divider field
                  + (SYSDIV2<<22);      // configure for 80 MHz clock
  // 5) wait for the PLL to lock by polling PLLLRIS
  while((SYSCTL_RIS_R&SYSCTL_RIS_PLLLRIS)==0){};
  // 6) enable use of PLL by clearing BYPASS
  SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2;
}
*/

//Initialize SysTick with bus wait running at bus clock
void SysTick_Init(void)
{
	NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
																				// enable SysTick with core clock
	//i.e. NVIC_ST_CTRL_R = 0x00000005
  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC;
}

//Initialize all three used ports
void PortBEF_Init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000032;		//Port B, E and F Run Mode Clock
	delay = SYSCTL_RCGC2_R;					//Delay for clock stabilization
	//Port B
	GPIO_PORTB_AMSEL_R &= 0x00;			//Disable analog function
	GPIO_PORTB_PCTL_R &= 0x00000000;//Clear bit PCTL
	GPIO_PORTB_DIR_R |= 0x3F;				//Set PB0-5 as outputs
	GPIO_PORTB_AFSEL_R &= 0x00;     //No alternate function
	GPIO_PORTB_DEN_R |= 0x3F;				//Digital Enable PB0-5
	//Port E
	GPIO_PORTE_AMSEL_R &= 0x00;			//Disable analog function
	GPIO_PORTE_PCTL_R &= 0x00000000;//Clear bit PCTL
	GPIO_PORTE_DIR_R &= ~0x07;			//Set PE0-2 as inputs
	GPIO_PORTE_AFSEL_R &= 0x00;     //No alternate function
	GPIO_PORTE_DEN_R |= 0x07;				//Digital Enable PE0-2
	//Port F
	GPIO_PORTF_AMSEL_R &= 0x00;			//Disable analog function
	GPIO_PORTF_PCTL_R &= 0x00000000;//Clear bit PCTL
	GPIO_PORTF_DIR_R |= 0x0A;				//Set PF1 & PF3 as outputs
	GPIO_PORTF_AFSEL_R &= 0x00;     //No alternate function
	GPIO_PORTF_DEN_R |= 0x0A;				//Digital Enable PF1 & PF3
}

//Uses SysTick to count down 1ms*(passed value)
void SysTick_Wait1ms(unsigned long ms)
{
	//Clock is set to 80Mhz, each SysTick takes 12.5 ns
	//To get 1ms count down delay, take (1 ms)/(12.5 ns)
	unsigned long delay = 80000;
	unsigned long i;
	for(i = 0; i < ms; i++)
	{
		NVIC_ST_RELOAD_R = delay - 1;	//The number of counts to wait
		NVIC_ST_CURRENT_R= 0;					//Clears current
		//Bit 16 of STCTRL is set to 1 if SysTick timer counts down to zero
		while((NVIC_ST_CTRL_R&0x00010000) == 0){}	
	}
}

