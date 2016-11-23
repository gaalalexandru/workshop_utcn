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
#include "display.h"
#include "sensor.h"

extern uint32_t Baud_Rate_Read;

int main(void)
{
	unsigned long ui32SysClock;
	
	IntMasterDisable();	//Global interrupt disable
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //80 Mhz
	ui32SysClock = SysCtlClockGet();
	RGB_PWM_Init(1000);
	TIMER_Wide_0_Init();
	TIMER_1_Init();
	TIMER_2_Init();
	Switch_Init();
	Display_Init();
	Sensor_Init();
	IntMasterEnable();	//Global interrupt enable
	
	Display_NewLine();  
	Display_NewLine();
	Display_String("Clock: ");
	Display_Decimal(ui32SysClock/1000000);
	Display_String(" MHz");
	Display_NewLine();
	Display_String("Baud Rate: ");
	Display_Decimal(Baud_Rate_Read);
	Display_String(" bit/sec");
	Display_NewLine();
	
	while(ui32SysClock)  //Clock working :)
	{
		//run forever
	}
	return 0;
}
//EOF
