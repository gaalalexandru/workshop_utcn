#ifndef TIMER
#define TIMER

/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"

/*-----------External function definisions-----------*/
void TIMER_delay(unsigned long delay_time_ms);
void TIMER_delay_No_Int(unsigned long delay_time_ms);
void TIMER_Wide_0_init(void);

#endif
//EOF
