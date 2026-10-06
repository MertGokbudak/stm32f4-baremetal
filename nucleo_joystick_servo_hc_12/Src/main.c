#define STM32F446xx
#include "stm32f4xx.h"

int main (void){

	uint32_t joy_x;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN | RCC_APB1ENR_USART2EN ;
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_USART1EN;

	//TIM3 PWM PA6 AF2
	int pin_t3 = 6;
	GPIOA->MODER &= ~(3U << (pin_t3*2));
	GPIOA->MODER |= (2U << (pin_t3*2));

	GPIOA->AFR[0] &= ~(15U << (pin_t3*4));
	GPIOA->AFR[0] |= (2U << (pin_t3*4));

	//TIM3_CCMR1_OC1M PWM mode 1
	TIM3->CCMR1 &= ~(7U << 4);
	TIM3->CCMR1 |= (6U << 4);

	//channel 1 enable
	TIM3->CCER  |=  TIM_CCER_CC1E;

	TIM3->PSC = 15;
	TIM3->ARR = 19999;
	TIM3->CCR1 = 1000 ;
	// Re-initialize the counter and generates an update of the registers.
	TIM3->EGR  = TIM_EGR_UG;
	//counter enable
	TIM3->CR1 |= TIM_CR1_CEN;


	//pinA0,A1 ->analog
	GPIOA->MODER &= ~(15U << 0);
	GPIOA->MODER |= (3U << 0) | (3U << 2);
	//ADON A/D Converter ON / OFF
	ADC1->CR2 &= ~(1U << 0);
	ADC1->CR2 |= (1U << 0);
	//Continuous conversion
	ADC1->CR2 &= ~(1U << 1);
	ADC1->CR2 |= (1U << 1);

	GPIOA->MODER &= ~(15U << 4);
	GPIOA->MODER |= (2U << 4) | (2U << 6);

	GPIOA->AFR[0] &= ~((15U << 8) | (15U << 12));
	GPIOA->AFR[0] |= (7U << 8) | (7U << 12);

	//TX,RX enable
	USART2->CR1 &= ~((1U << 2) |(1U << 3));
	USART2->CR1 |= ((1U << 2) |(1U << 3));

	USART2->BRR = 1667;

	//USART enable
	USART2->CR1 &= ~(1U << 13);
	USART2->CR1 |= (1U << 13);

	GPIOA->MODER &= ~(3U << (9*2));
	GPIOA->MODER |= (2U << (9*2));

	GPIOA->AFR[1] &= ~(15U << 4);
	GPIOA->AFR[1] |= (7U << 4);

	USART1->CR1 &= ~(1U << 3);
	USART1->CR1 |= (1U << 3);

	USART1->BRR = 1667;

	USART1->CR1 &= ~(1U << 13);
	USART1->CR1 |= (1U << 13);

	//SWSTART: Start conversion of regular channels
	ADC1->CR2 &= ~(1U << 30);
	ADC1->CR2 |= (1U << 30);

	while (1){


		while (!(ADC1->SR & ADC_SR_EOC));//1: Conversion complete -- Bit 1 EOC: Regular channel end of conversion

		joy_x = ADC1->DR;
		TIM3->CCR1 =(joy_x * 500/2048) + 1000 ;

		int jx_modified = 0;
		uint8_t numbers_reversed[5];

		//Status register (USART_SR) Address offset: 0x00 Bit 7 TXE: Transmit data register empty Bit 5 RXNE: Read data register not empty

		jx_modified = joy_x >> 4;//12 bit to 8 bit
		int i = 0;

		while(1){
			numbers_reversed[i] = (jx_modified % 10);
			jx_modified = (jx_modified / 10);
			i++;

			if (jx_modified  == 0 ){
				break;
			}
		}
		for (int t = i; t > 0; t--){
			while (!(USART2->SR & (1U << 7)));
			USART2->DR = numbers_reversed[t-1] + '0';
		}
		while (!(USART2->SR & (1U << 7)));
		USART2->DR = '\n';

		while (!(USART1->SR & (1U << 7)));
		USART1->DR = joy_x >> 4; //12 bit to 8 bit

	}

}
