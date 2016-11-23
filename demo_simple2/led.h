#ifndef LED
#define LED

/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"

/*-------------------Macro Definitions----------------*/
#define RED (0x02)	//PF1
#define BLUE (0x04)	//PF2
#define GREEN (0x08)  //PF3

/*-----------External function definisions-----------*/
void RGB_Led_Init(void);
void RGB_PWM_Init(unsigned long PWM_Period);
void RGB_Led_toggle(unsigned char color);

void RED_PWM_DutyCycle(unsigned long duty_cycle);
void GREEN_PWM_DutyCycle(unsigned long duty_cycle);
void BLUE_PWM_DutyCycle(unsigned long duty_cycle);

#endif
//EOF
