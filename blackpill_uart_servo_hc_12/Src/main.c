#include <stdint.h>

# define GPIOC_BASE 0x40020800
# define RCC_BASE 0x40023800
# define RCC_AHB1ENR (*(volatile uint32_t *) (RCC_BASE + 0x30))
# define GPIOC_MODER (*(volatile uint32_t *) (GPIOC_BASE + 0x00))
# define GPIOC_ODR (*(volatile uint32_t *) (GPIOC_BASE + 0x14))

# define GPIOA_MODER (*(volatile uint32_t *) 0x40020000)
# define GPIOA_AFRL (*(volatile uint32_t *) (0x40020000 + 0x20))
# define GPIOA_AFRH (*(volatile uint32_t *) (0x40020000 + 0x24))
# define RCC_APB2ENR (*(volatile uint32_t *) (0x40023800 + 0x44))

# define USART1_Base 0x40011000
# define USART1_DR (*(volatile uint32_t *) (USART1_Base + 0x04))
# define USART1_CR1 (*(volatile uint32_t *) (USART1_Base + 0x0C))
# define USART1_BRR (*(volatile uint32_t *) (USART1_Base + 0x08))
# define USART1_SR (*(volatile uint32_t *) (USART1_Base + 0x00))

# define RCC_APB1ENR (*(volatile uint32_t *) (0x40023800 + 0x40))
# define TIM3_CR1 (*(volatile uint32_t *) (0x40000400 + 0x00))//Auto-reload preload enable
# define TIM3_CCMR1 (*(volatile uint32_t *) (0x40000400 + 0x18))//TIMx capture/compare mode register 1 PWM mode 1
# define TIM3_CCER (*(volatile uint32_t *) (0x40000400 + 0x20))
# define TIM3_PSC (*(volatile uint32_t *) (0x40000400 + 0x28))
# define TIM3_ARR (*(volatile uint32_t *) (0x40000400 + 0x2C))
# define TIM3_CCR1 (*(volatile uint32_t *) (0x40000400 + 0x34))

int main(void){
	//LED
	//Bit 2GPIOCEN: IO port C clock enable
	RCC_AHB1ENR &= ~(1U <<2);
	RCC_AHB1ENR |= (1U <<2);

	GPIOC_MODER &= ~(3U <<(13*2));
	GPIOC_MODER |= (1U <<(13*2));

	//USART1
	//USART1 TX
	RCC_AHB1ENR &= ~(1U << 0);
	RCC_AHB1ENR |= (1U << 0);

	RCC_APB2ENR &= ~(1U << 4);
	RCC_APB2ENR |= (1U << 4);

	GPIOA_MODER &= ~(3U << (10*2));
	GPIOA_MODER |= (2U << (10*2));

	GPIOA_AFRH &= ~(15U << 8);
	GPIOA_AFRH |= (7U << 8);

	USART1_CR1 &= ~(1U << 2);
	USART1_CR1 |= (1U << 2);

	USART1_BRR = 1667;

	USART1_CR1 &= ~(1U << 13);
	USART1_CR1 |= (1U << 13);
	//TIM3
	RCC_APB1ENR &= ~(1U << 1);
	RCC_APB1ENR |= (1U << 1);

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
	TIM3_CCR1 = 1500 ;

	//TIM3_CR1_CEN
	TIM3_CR1 &= ~(1U << 0);
	TIM3_CR1 |= (1U << 0);


	while(1){
		while (USART1_SR & (1U << 5)){//Bit 5 RXNE: Read data register not empty
			uint8_t received_data = USART1_DR;
			TIM3_CCR1 =(received_data * 1000/255) + 1000 ;
			/*0if (received_data == 'A'){
				GPIOC_ODR ^= (1U << 13);}
			for (volatile int i = 0; i < 500000; i++);*/

		}
	}
}
