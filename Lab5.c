/*====================================================*/
/* Ryan Nguyen and Tyler Bishop */
/* ELEC 3040/3050 - Lab 3, Program 1 */
/*====================================================*/
#include "STM32L1xx.h"

int count;
int key;
int reset;
int dummy;

void PinSetup () {
	RCC->AHBENR |= 0x01; // Sets up the Inputs
	GPIOA->MODER &= ~(0x0000000C); // Setups up PA1 as an Input

	RCC->AHBENR |= 0x04; // Sets up the LED Outputs
	GPIOC->MODER &= ~(0x000000FF); 
	GPIOC->MODER |= (0x00000055); 
	
	RCC->AHBENR |= 0x02; // Sets up the Keypad
	GPIOB->MODER &= ~(0x0000FFFF);
	GPIOB->MODER |= (0x00005500);
	GPIOB->ODR = 0;
	GPIOB->PUPDR &= ~(0x000000FF);
	GPIOB->PUPDR |= (0x00000055);
	
	NVIC_EnableIRQ (EXTI1_IRQn);
	NVIC_ClearPendingIRQ (EXTI1_IRQn); 

	SYSCFG->EXTICR[0] &= 0xFF0F;
	SYSCFG->EXTICR[0] |= 0x0000; 

	EXTI->FTSR |=0x0002;
	EXTI->IMR |=0x0002;
	

	__enable_irq();
}

void delay (double seconds) {
	int i,j,n;
	if (reset > 0) {
		reset--;
	}
	for (i=0; i<350000*seconds; i++) {
		n = i;
	}
}

int readColumn() {
	GPIOB->MODER &= ~(0x0000FFFF);
	GPIOB->MODER |= (0x00000055);
	GPIOB->ODR = 0;
	GPIOB->PUPDR &= ~(0x0000FF00);
	GPIOB->PUPDR |= (0x00005500);
	dummy = 4;
	while (dummy > 0) {
		dummy--;
	}
	int input = GPIOB->IDR&0xF0;
	switch(input) {
		case 0xE0:
			return 1;
		case 0xD0:
			return 2;
		case 0xB0:
			return 3;
		case 0x70:
			return 4;
		default:
			return -1;
	}
}

int readRow() {
	GPIOB->MODER &= ~(0x0000FFFF);
	GPIOB->MODER |= (0x00005500);
	GPIOB->ODR = 0;
	GPIOB->PUPDR &= ~(0x000000FF);
	GPIOB->PUPDR |= (0x00000055);
	dummy = 4;
	while (dummy > 0) {
		dummy--;
	}
	int input = GPIOB->IDR&0xF;
	switch(input) {
		case 0xE:
			return 1;
		case 0xD:
			return 2;
		case 0xB:
			return 3;
		case 0x7:
			return 4;
		default:
			return -1;
	}
}

void EXTI1_IRQHandler () {
	 //if (reset == 0) {
		int row = readRow();
		int col = readColumn();
		
		dummy=5;
		switch (row) {
			case 1:
				switch (col) {
					case 1:
						key = 1;
						break;
					case 2:
						key = 2;
						break;
					case 3:
						key = 3;
						break;
					case 4:
						key = 10;
						break;
					default:
						key = -1;
				}
			break;
				case 2:
				switch (col) {
					case 1:
						key = 4;
						break;
					case 2:
						key = 5;
						break;
					case 3:
						key = 6;
						break;
					case 4:
						key = 11;
						break;
					default:
						key = -1;
				}
			break;
				case 3:
				switch (col) {
					case 1:
						key = 7;
						break;
					case 2:
						key = 8;
						break;
					case 3:
						key = 9;
						break;
					case 4:
						key = 12;
						break;
					default:
						key = -1;
				}
			break;
				case 4:
				switch (col) {
					case 1:
						key = 15;
						break;
					case 2:
						key = 0;
						break;
					case 3:
						key = 14;
						break;
					case 4:
						key = 13;
						break;
					default:
						key = -1;
				}
			break;
					default:
						key = -1;
		}
	
		if (key != -1) {
			reset = 5;
		} else {
			reset = 0;
		}
		
		RCC->AHBENR |= 0x02; // Sets up the Keypad
		GPIOB->MODER &= ~(0x0000FFFF);
		GPIOB->PUPDR &= ~(0x0000FF00);
		GPIOB->PUPDR |= (0x00005500);
	 //}
	
	
	GPIOB->MODER &= ~(0x0000FFFF);
	GPIOB->MODER |= (0x00005500);
	GPIOB->ODR = 0;
	GPIOB->PUPDR &= ~(0x000000FF);
	GPIOB->PUPDR |= (0x00000055);
	NVIC_ClearPendingIRQ(EXTI1_IRQn);
	EXTI->PR |= 0x0002;
}

void counter() {
	if (count != 9) { 
		count++; 
	} else {
		count = 0; 
	}
}

void display(int input) {
	GPIOC->ODR = input;
}

/*------------------------------------------------*/
/* Main program */
/*------------------------------------------------*/
int main(void) {
	
	PinSetup(); 
	count = 0; 
	key = 0;
	reset = 0;
	
	display(count);
	while (1) {
		 
		counter();
		delay(0.05); 
		if (reset == 0) {
			display(count);
		} else {
			display(key);
		}
		
		
	}
} 