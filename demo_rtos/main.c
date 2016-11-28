// main.c - ssOS usage example
// Runs on LM4F120/TM4C123 will be ported to MSP432 in next version
// ssOS - stupid simple Operating System
// A very simple real time operating system with minimal features.
// For copyright, configuration and usage read readme.txt

#include <stdint.h>
#include "os_core.h"
#include "os_hw.h"
#include "profile.h"
//#include "../inc/tm4c123gh6pm.h"  //AleGaa not needed at the moment

#include "driverlib/gpio.h"
#include "driverlib/adc.h"
/*------------------Project Includes-----------------*/
#include "led.h"
#include "display.h"
#include "sensor.h"
#include "timer_handler.h"

#define THREADFREQ (TIMER_reload_calculator(1))   // reload value for 1 ms -> frequency in Hz

fifo_t FifoA_Temperature;
fifo_t FifoB_Voltage;
int8_t PWM_DutyCycle = 50;
uint32_t ui32SysClock;

extern ptcbType PerTask[NUMPERIODIC];
extern PortSema_t SemPortF;
extern uint32_t Baud_Rate_Read;

void Task0_Display(void){	//Display task
uint32_t temp, volt;
	
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
	Display_NewLine();
	
  while(1){
		//OS_Wait(&PerTask[0].semaphore);
		temp = OS_FIFO_Get(&FifoA_Temperature);
		volt = OS_FIFO_Get(&FifoB_Voltage);
		
    Profile_Toggle0();
		//Display content:
		Display_NewLine();
		Display_String("Internal temperature: ");
		IntMasterDisable();	//Global interrupt disable
		Display_Decimal(temp);
		IntMasterEnable();	//Global interrupt enable
		Display_String(" degC");	
		Display_NewLine();
		Display_String("Analog voltage: ");
		IntMasterDisable();	//Global interrupt disable
		Display_Decimal(volt);
		IntMasterEnable();	//Global interrupt enable
		Display_String(" mV");
		Display_NewLine();
		Display_NewLine();	
		OS_Sleep(1000);
  }
}
void Task1_ADCTrigger(void){		 //ADC trigger task
  while(1){
		OS_Wait(&PerTask[0].semaphore);
    Profile_Toggle1();
		ADCProcessorTrigger(ADC0_BASE, 3);  //Trigger Temperature sensor ADC
  }
}
void Task2_SW2_PF0(void){		//Edge triggered task PF0
  while(1){
		OS_Wait(&SemPortF.pin0); // signaled in OS on button touch
		OS_Sleep(50); //sleep to debounce switch		
		if(!GPIOPinRead(GPIO_PORTF_BASE,GPIO_INT_PIN_0)) {   
			Profile_Toggle2();
			if (PWM_DutyCycle >= 99)
			{
				Display_String("HOT enough!");
				Display_NewLine();
			}
			else
			{
				Display_String("Getting hotter...");	
				Display_String(" Simulated temperature: ");
				Display_Decimal(PWM_DutyCycle);
				Display_String(" % of 100%");
				Display_NewLine();
			}
			PWM_DutyCycle += 2;
			if (PWM_DutyCycle > 100) {
				PWM_DutyCycle = 100;
			}
			RED_PWM_DutyCycle(PWM_DutyCycle);
			BLUE_PWM_DutyCycle(100-PWM_DutyCycle);	
		}
		OS_EdgeTrigger_Restart(PortF,GPIO_PIN_0);
	}
}
void Task3_SW1_PF4(void){	 //Edge triggered task PF4

  while(1){
		OS_Wait(&SemPortF.pin4); // signaled in OS on button touch
		OS_Sleep(50); //sleep to debounce switch		
		if(!GPIOPinRead(GPIO_PORTF_BASE,GPIO_INT_PIN_4)) {   
			Profile_Toggle4();
			if (PWM_DutyCycle == 0)
			{
				Display_String("COLD enough!");
				Display_NewLine();
			}
			else
			{
				Display_String("Getting colder...");
				Display_String(" Simulated temperature: ");
				Display_Decimal(PWM_DutyCycle);
				Display_String(" % of 100%");				
				Display_NewLine();
			}
			PWM_DutyCycle -= 2;
			if (PWM_DutyCycle < 0) {
				PWM_DutyCycle = 0;
			}
			RED_PWM_DutyCycle(PWM_DutyCycle);
			BLUE_PWM_DutyCycle(100-PWM_DutyCycle);
		}
		OS_EdgeTrigger_Restart(PortF,GPIO_PIN_4);
	}
}

void Idle_Task(void){while(1){}}

int main(void){

	OS_Init();            // initialize, disable interrupts
	Profile_Init();       // enable digital I/O on profile pins
	ui32SysClock = SysCtlClockGet();
	
	//Application init
	Display_Init();
	Sensor_Init();	
	RGB_PWM_Init(1000);	

	OS_InitSemaphore(&SemPortF.pin0,0);
	OS_InitSemaphore(&SemPortF.pin4,0);	
	
	OS_FIFO_Init(&FifoA_Temperature);
	OS_FIFO_Init(&FifoB_Voltage);

	OS_EdgeTrigger_Init(PortF,GPIO_PIN_0|GPIO_PIN_4,0,GPIO_LOW_LEVEL,GPIO_PIN_TYPE_STD_WPU);
//OS_EdgeTrigger_Init( port,  pin,  priority,  type, resistor)
	
	//OS_AddPeriodicEventThread(&PerTask[0].semaphore, 500); //Display thread
	OS_AddPeriodicEventThread(&PerTask[0].semaphore, 100); //ADC sampling thread

  OS_AddThreads(&Task0_Display,1, 
	              &Task1_ADCTrigger, 2,
	              &Task2_SW2_PF0, 5,
	              &Task3_SW1_PF4, 5,
	              &Idle_Task,254);	//Idle task is lowest priority
	
  OS_Launch(79999+1); // doesn't return, interrupts enabled in here
  return 0;             // this never executes
}
//EOF
