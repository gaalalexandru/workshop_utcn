/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"

/*-------------------Driver Includes-----------------*/
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

/*------------------Project Includes-----------------*/
#include "led.h"
#include "timer.h"
#include "switch.h"

unsigned char Led_Color;

int main(void)
{
	unsigned long ui32SysClock;

	IntMasterDisable();	//Global interrupt disable
	Led_Color = 0;	//LED Off
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //80 Mhz
	ui32SysClock = SysCtlClockGet();
	RGB_Led_Init();
	TIMER_Wide_0_init();
	Switch_Init();
	IntMasterEnable();	//Global interrupt enable
	while(ui32SysClock)  //Clock working :)
	{
		//run forever
	}
	return 0;
}
//EOF
