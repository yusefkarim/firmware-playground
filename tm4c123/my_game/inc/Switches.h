#ifndef SWITCHES_H
#define SWITCHES_H

#define Walk      (GPIO_PORTE_DATA_R&0x01)
#define Block     (GPIO_PORTE_DATA_R&0x02)
#define Attack    (GPIO_PORTE_DATA_R&0x04)

//Read the interrupt status for each of the three buttons
#define WalkButton      (GPIO_PORTE_RIS_R&0x01)
#define BlockButton     (GPIO_PORTE_RIS_R&0x02)
#define AttackButton    (GPIO_PORTE_RIS_R&0x04)

//Acknowledge the interrupt flag and clear it
#define WalkButtonClear      GPIO_PORTE_ICR_R|=0x01
#define BlockButtonClear     GPIO_PORTE_ICR_R|=0x02
#define AttackButtonClear    GPIO_PORTE_ICR_R|=0x04

//------------Switches_Init------------//
//Setup port E, PE2-0
void Switches_Init(void);

#endif
