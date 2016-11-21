/*------------------Project Includes-----------------*/
#include "led.h"

/*-------------------Driver Includes-----------------*/
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

/*-------------------HW define Includes--------------*/
#include "inc/hw_memmap.h"

/*-------------------Macro Definitions----------------*/
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02 - 1
// blue     --B    0x04 - 2
// green    -G-    0x08 
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E
// pink     R-B    0x06


void RGB_Led_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  //Enable clock on port F
  GPIO_PORTF_CR_R |= 0x1F;  //Allow changes to PF4-0
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);  //Set PF1,2,3 GPIO Output
	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_DIR_MODE_OUT);  //Set direction Output for PF1,2,3
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);  //Configure PF1,2,3
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);  //Set initially to 0
}

void Led_toggle(unsigned char color)
{
	static unsigned char status = 1;
	if(status) {
		GPIOPinWrite(GPIO_PORTF_BASE, color, color);	//switch on desired color	
	}
	else {
		GPIOPinWrite(GPIO_PORTF_BASE, color, 0);  //switch off
	}
	status ^= 1;  //toggle status
}
//EOF
