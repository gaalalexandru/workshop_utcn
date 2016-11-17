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
void Led_toggle(unsigned char color);

#endif
//EOF
