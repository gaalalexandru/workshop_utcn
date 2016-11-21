#include "sensor.h"

/*-------------------Driver Includes-----------------*/
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"

/*-------------------HW define Includes--------------*/
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

/*-------------------Macro Definitions----------------*/
#define ADC0SS3_PRIO (0x02)  //Priority 2
#define ADC_Ref_Voltage (33)	//33 represents 3v3, 50 represents 5v

unsigned long Sensor_Temperature = 0;
unsigned long Sensor_AnalogVoltage = 0;

void Sensor_Init(void) //Initialize temperature sensor input
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);  //The ADC0 peripheral must be enabled for use.

	IntDisable(INT_ADC0SS3);
	
	ADCIntClear(ADC0_BASE, 3);  //Clear interrupt flag	
	ADCIntDisable(ADC0_BASE, 3);
	ADCSequenceDisable(ADC0_BASE,3);  //Disable sequencer 3
	
	ADCSequenceConfigure(ADC0_BASE,3, /*ADC_TRIGGER_ALWAYS*/ADC_TRIGGER_PROCESSOR, 0); //Sequence 3 will do a single sample when the processor (SW) sends a signal to start the conversion
	ADCSequenceStepConfigure(ADC0_BASE,3,0, ADC_CTL_CH3 |ADC_CTL_TS| ADC_CTL_IE | ADC_CTL_END);  //Configure step 0 on sequencer 3 to channel 1, interrupt enable and end conversion when after 1 conversion
	
	ADCSequenceEnable(ADC0_BASE,3);  //Enable sequencer 3
	ADCIntEnable(ADC0_BASE, 3); 
		
	IntPrioritySet(INT_ADC0SS3,(ADC0SS3_PRIO)<<5);
	IntEnable(INT_ADC0SS3);	
}

void ADC0SS3_Handler(void) {  //ADC0 Seq3 ISR

	uint32_t Voltage;
	if(ADCIntStatus(ADC0_BASE, 3, false))
	{
		ADCIntClear(ADC0_BASE, 3);  //Clear interrupt flag
		ADCSequenceDataGet(ADC0_BASE, 3, &Voltage);		
		
		IntMasterDisable();	//Global interrupt disable
		//Sensor_Temperature = (1475 - ((75 * (ADC_Ref_Voltage) * Voltage) / 4096))/10;
		Sensor_Temperature = 147.5 - ((75 * (3.3) * Voltage) / 4096);
		Sensor_AnalogVoltage = Voltage;
		IntMasterEnable();	//Global interrupt enable
		
		//Sensor_Temperature = Voltage; //Gaal Alexandru use this for RAW ADC value
		
		/*
		The internal temperature sensor converts a temperature measurement into a voltage. This voltage
		value, VTSENS, is given by the following equation (where TEMP is the temperature in °C):
		VTSENS = 2.7 - ((TEMP + 55) / 75)
		
		The temperature sensor reading can be sampled in a sample sequence by setting the TSn bit in
		the ADCSSCTLn register. The temperature reading from the temperature sensor can also be given
		as a function of the ADC value. The following formula calculates temperature (TEMP in ?) based
		on the ADC reading (ADCCODE, given as an unsigned decimal number from 0 to 4095) and the
		maximum ADC voltage range (VREFP - VREFN):
		TEMP = 147.5 - ((75 * (VREFP - VREFN) × ADCCODE) / 4096)
		*/
	}
}

//EOF
