#include <stdint.h>

# define RCC_APB1ENR (*(volatile uint32_t *) (0x40023800 + 0x40))
# define RCC_AHB1ENR (*(volatile uint32_t *) (0x40023800 + 0x30))
# define RCC_APB2ENR (*(volatile uint32_t *) (0x40023800 + 0x44))//USART1,ADC

# define GPIOA_MODER (*(volatile uint32_t *) 0x40020000)
# define GPIOA_AFRL (*(volatile uint32_t *) (0x40020000 + 0x20))
# define GPIOA_AFRH (*(volatile uint32_t *) (0x40020000 + 0x24))

# define TIM3_CR1 (*(volatile uint32_t *) (0x40000400 + 0x00))//Auto-reload preload enable
# define TIM3_CCMR1 (*(volatile uint32_t *) (0x40000400 + 0x18))//TIMx capture/compare mode register 1 PWM mode 1
# define TIM3_CCER (*(volatile uint32_t *) (0x40000400 + 0x20))
# define TIM3_PSC (*(volatile uint32_t *) (0x40000400 + 0x28))
# define TIM3_ARR (*(volatile uint32_t *) (0x40000400 + 0x2C))
# define TIM3_CCR1 (*(volatile uint32_t *) (0x40000400 + 0x34))

# define ADC1_BASE 0x40012000
# define ADC_CR2 (*(volatile uint32_t *) (0x40012000 + 0x08))
# define ADC_SQR3 (*(volatile uint32_t *) (0x40012000 + 0x34))
# define ADC_SR (*(volatile uint32_t *) 0x40012000)
# define ADC_DR (*(volatile uint32_t *) (0x40012000 + 0x4C))

# define USART2_Base 0x40004400
# define USART2_DR (*(volatile uint32_t *) (USART2_Base + 0x04))
# define USART2_CR1 (*(volatile uint32_t *) (USART2_Base + 0x0C))
# define USART2_BRR (*(volatile uint32_t *) (USART2_Base + 0x08))
# define USART2_SR (*(volatile uint32_t *) (USART2_Base + 0x00))

# define USART1_Base 0x40011000
# define USART1_DR (*(volatile uint32_t *) (USART1_Base + 0x04))
# define USART1_CR1 (*(volatile uint32_t *) (USART1_Base + 0x0C))
# define USART1_BRR (*(volatile uint32_t *) (USART1_Base + 0x08))
# define USART1_SR (*(volatile uint32_t *) (USART1_Base + 0x00))


int main (void){

	uint32_t joy_x;

	RCC_APB1ENR &= ~(1U << 1);
	RCC_APB1ENR |= (1U << 1);

	RCC_AHB1ENR &= ~(1U << 0);
	RCC_AHB1ENR |= (1U << 0);

	GPIOA_MODER &= ~(3U << 12);
	GPIOA_MODER |= (2U <<12);

	GPIOA_AFRL &= ~(15U << 24);
	GPIOA_AFRL |= (2U << 24);
	//TIM3_CR1_ARPE
	TIM3_CR1 &= ~(1U << 7);
	TIM3_CR1 |= (1U << 7);
	//TIM3_CCMR1_OC1M
	TIM3_CCMR1 &= ~(7U << 4);
	TIM3_CCMR1 |= (6U << 4);
	//TIM3_CCER_CC1E
	TIM3_CCER &= ~(1U << 0);
	TIM3_CCER |= (1U << 0);

	TIM3_PSC = 15;
	TIM3_ARR = 19999;
	TIM3_CCR1 = 1000 ;

	//TIM3_CR1_CEN
	TIM3_CR1 &= ~(1U << 0);
	TIM3_CR1 |= (1U << 0);

	//ADC1EN
	RCC_APB2ENR &= ~(1U << 8);
	RCC_APB2ENR |= (1U << 8);
	//pinA0,A1 ->analog
	GPIOA_MODER &= ~(15U << 0);
	GPIOA_MODER |= (3U << 0) | (3U << 2);
	//ADON A/D Converter ON / OFF
	ADC_CR2 &= ~(1U << 0);
	ADC_CR2 |= (1U << 0);

	//USART 2 TX,RX
	RCC_APB1ENR &= ~(1U << 17);
	RCC_APB1ENR |= (1U << 17);

	GPIOA_MODER &= ~(15U << 4);
	GPIOA_MODER |= (2U << 4) | (2U << 6);

	GPIOA_AFRL &= ~((15U << 8) | (15U << 12));
	GPIOA_AFRL |= (7U << 8) | (7U << 12);

	//TX,RX enable
	USART2_CR1 &= ~((1U << 2) |(1U << 3));
	USART2_CR1 |= ((1U << 2) |(1U << 3));

	USART2_BRR = 1667;

	//USART enable
	USART2_CR1 &= ~(1U << 13);
	USART2_CR1 |= (1U << 13);

	//USART1 TX
	RCC_APB2ENR &= ~(1U << 4);
	RCC_APB2ENR |= (1U << 4);

	GPIOA_MODER &= ~(3U << (9*2));
	GPIOA_MODER |= (2U << (9*2));

	GPIOA_AFRH &= ~(15U << 4);
	GPIOA_AFRH |= (7U << 4);

	USART1_CR1 &= ~(1U << 3);
	USART1_CR1 |= (1U << 3);

	USART1_BRR = 1667;

	USART1_CR1 &= ~(1U << 13);
	USART1_CR1 |= (1U << 13);

	while (1){

		//SQ1 5bit
		ADC_SQR3 &= ~(31U << 0);
		//PA0-0U,PA1-1U
		ADC_SQR3 |= (0U << 0);
		//SWSTART: Start conversion of regular channels
		ADC_CR2 &= ~(1U << 30);
		ADC_CR2 |= (1U << 30);


		while (! (ADC_SR & (1U <<1)));//1: Conversion complete

		joy_x = ADC_DR;
		TIM3_CCR1 =(joy_x * 500/2048) + 1000 ;

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
			while (!(USART2_SR & (1U << 7)));
			USART2_DR = numbers_reversed[t-1] + '0';
		}
		while (!(USART2_SR & (1U << 7)));
		USART2_DR = '\n';

		while (!(USART1_SR & (1U << 7)));
		USART1_DR = joy_x >> 4; //12 bit to 8 bit

	}

}
