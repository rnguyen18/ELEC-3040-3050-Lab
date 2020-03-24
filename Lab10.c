//03.19.2020
//ELEC 3040/3050
//Lab 10 (Special)

//Read analog input on pin PA5 and convert using ADC

//Include statements
#include "STM32L1xx.h" 										//Microcontroller info

//Global variables
static int   adc_out;											//Global: analog voltage
static int   adc_values[100];							//Global: analog voltage array
static int   adc_index = 0;								//Global: adc_values index

//Initialization subroutines
void ClockSetup()													//Enable HSI Clock
{	
	RCC->CR |= RCC_CR_HSION;								//Turn on 16MHz HSI oscillator
	while ((RCC->CR & RCC_CR_HSIRDY) == 0); //Wait for HSI to be ready
}

void PinSetup() 													//Configure Pin PA5
{
	RCC->AHBENR |= 0x01; //Enable GPIOA clock
	
	GPIOA->MODER &= ~(0x00000003);		//Set PA5 to analog mode								
}

void ADCSetup() 													//Configure ADC
{
	RCC->APB2ENR |= 0x00000200;	//Enable ADC1 clock											
	ADC1->SQR5		&= ~(0x1F);								//Clear ADC1 sequence register
	ADC1->SQR5 != 0x00000005;																	//Set ADC1 to channel 5									
	ADC1->CR2 |= 1; 	//Turn ADC1 on																		
	while ((ADC1->SR & 0x02) == 0); 			//Wait for ADC1 to be ready							
}

void TimerSetup()													//Configure Timer 11 for 1kHz Sampling
{
	RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;	//Enable TIM11 clock										
	TIM11->PSC = 2											  	//Set prescaler value										
	TIM11->ARR = 5242									    	//Set auto-reload value									
	TIM11->CNT = 0; 												//Initialize counter value
	NVIC_ClearPendingIRQ(TIM11_IRQn);	//Clear TIM11 pending flag							
	NVIC_EnableIRQ(TIM11_IRQn);	//Enable TIM11 interrupts	
	TIM11->CR1 |= TIM_CR1_CEN;	//Enable timer counting									
	NVIC_EnableIRQ(EXTI11_IRQn);							//Enable TIM11 NVIC interrupts					
	NVIC_ClearPendingIRQ(EXTI11_IRQn);				//Clear TIM11 NVIC pending interrupts   
}

//Interrupt handlers
void TIM11_IRQHandler()
{
	ADC1->CR2 |= 0x40000000																//Start conversion											
	while(ADC1->CR2 & 0x400 == 0);	        //Wait for conversion to complete				
	adc_out = ADC1->DR										 					//Read individual ADC result						
	if (adc_index < 100)									    	//Check for 100 samples									
	{
		adc_values[adc_index++] = adc_out	//Write results to output array					
	}
	TIM11->SR &= ~TIM_SR_UIF;	//Clear TIM11 pending flag     					
	NVIC_ClearPendingIRQ(TIM11_IRQn);		 		//Clear TIM11 NVIC pending interrupts 	
}

//Main program
int main(void) 														//Main program
{
	ClockSetup();														//Enable HSI clock
	PinSetup(); 														//Configure GPIO pins
	ADCSetup();															//Configure ADC
	TimerSetup();														//Configure timer
	TIM11->DIER |= TIM_DIER_UIE;		//CPU interrupt enable									
	while(1);																//Endless loop (do nothing)
}
