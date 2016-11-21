/*------------------Project Includes-----------------*/
#include "switch.h"
#include "led.h"
#include "timer.h"
#include "display.h"

/*-------------------Driver Includes-----------------*/
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"

/*-------------------HW define Includes--------------*/
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

/*-------------------Macro Definitions----------------*/
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define SWITCH_PRIO (0x01)

extern unsigned char Led_Color;

void Switch_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  //Enable clock on port F
	
	GPIO_PORTF_LOCK_R = 0x4C4F434B;  //Unlock PortF PF0  
  GPIO_PORTF_CR_R |= 0x1F;  //Allow changes to PF4-0
	
	IntDisable(INT_GPIOF);  //GPIO Port F disable of interrupts
	
	GPIOIntDisable(GPIO_PORTF_BASE,GPIO_PIN_0 | GPIO_PIN_4);  //Disable GPIO pin interrupt
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);  //Set PF0 and PF4 as GPIO Input
	GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_FALLING_EDGE);  //Set Low level interrupt type
	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_DIR_MODE_IN);  //Set direction input for PF0 and PF4
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); //Configure PUR for PF0 and PF4
	GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_0 | GPIO_INT_PIN_4);  //Enable GPIO pin interrupt
	
	IntPrioritySet(INT_GPIOF,(SWITCH_PRIO)<<5);  //Priority 2 = "010"0.0000
	IntEnable(INT_GPIOF);  //GPIO Port F enable of interrupts
}

void GPIOF_Handler(void) 	//GPIO port F ISR
{
	unsigned long switch_status = 0;
	static unsigned short color_calculation = 0;
	switch_status = GPIOIntStatus(GPIO_PORTF_BASE,false);
	GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_0 | GPIO_INT_PIN_4);
	if((switch_status&(GPIO_INT_PIN_0 | GPIO_INT_PIN_4)) == (GPIO_INT_PIN_0 | GPIO_INT_PIN_4))
	{
		//Both switches are pressed

	}
	else if(switch_status & GPIO_INT_PIN_4)
	{
		//PF4
		color_calculation = ((color_calculation + 1) % 8);	//will have values from 0 to 7
		Led_Color = (color_calculation << 1);  //Change LED color, shift left by 1 bit because bit0 / PF0 is input
		Display_NewLine();
		Display_String("Color Changed!!!");
	}
	else if(switch_status & GPIO_INT_PIN_0)
	{
		//PF2
	}
	TIMER_delay_No_Int(10);  //Kind of debounce switch
}
//EOF
