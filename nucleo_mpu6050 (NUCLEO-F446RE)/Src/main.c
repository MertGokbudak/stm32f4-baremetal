#include <stdint.h>

# define RCC_APB1ENR (*(volatile uint32_t *) (0x40023800 + 0x40))
# define RCC_AHB1ENR (*(volatile uint32_t *) (0x40023800 + 0x30))
# define GPIOA_MODER (*(volatile uint32_t *) 0x40020000)
# define GPIOA_AFRL (*(volatile uint32_t *) (0x40020000 + 0x20))
# define USART2_Base 0x40004400
# define USART2_DR (*(volatile uint32_t *) (USART2_Base + 0x04))
# define USART2_CR1 (*(volatile uint32_t *) (USART2_Base + 0x0C))
# define USART2_BRR (*(volatile uint32_t *) (USART2_Base + 0x08))
# define USART2_SR (*(volatile uint32_t *) (USART2_Base + 0x00))

# define GPIOB_BASE 0x40020400
# define I2C1_BASE 0x40005400
# define GPIOB_MODER (*(volatile uint32_t *) (0x40020400 + 0x00))//10
# define GPIOB_AFRH (*(volatile uint32_t *) (0x40020400 + 0x24))//0100: AF4
# define GPIOB_OTYPER (*(volatile uint32_t *) (0x40020400 + 0x04))

# define I2C1_CR1 (*(volatile uint32_t *) (I2C1_BASE + 0x00))
//Bit 0 PE: Peripheral enable

# define I2C1_CR2 (*(volatile uint32_t *) (I2C1_BASE + 0x04))
//Bits 5:0 FREQ[5:0]: Peripheral clock frequency

# define I2C1_DR (*(volatile uint32_t *) (I2C1_BASE + 0x10))

# define I2C1_SR1 (*(volatile uint32_t *) (I2C1_BASE + 0x14))

# define I2C1_SR2 (*(volatile uint32_t *) (I2C1_BASE + 0x18))
# define I2C1_CCR (*(volatile uint32_t *) (I2C1_BASE + 0x1C))
//Bits 11:0 CCR[11:0]: Clock control register in Fm/Sm mode (controller mode)
# define I2C1_TRISE (*(volatile uint32_t *) (I2C1_BASE + 0x20))
//Bits 5:0 TRISE[5:0]: Maximum rise time in Fm/Sm mode (controller mode)


uint8_t I2C_read(uint8_t reg, uint8_t adress){

	I2C1_CR1 |= (1U << 8); //Bit 8 START: Start generation
	while (!( I2C1_SR1 & (1U <<0))); //Bit 0 SB: Start bit (controller mode)
	I2C1_DR = (adress << 1) | 0 ;//Bits 7:0 DR[7:0] 8-bit data register
	while (!(I2C1_SR1 & (1U <<1))); //Bit 1 ADDR: Address sent (controller mode)/matched (target mode)
	(void)I2C1_SR1;
	(void)I2C1_SR2;
	I2C1_DR = reg;
	while (!(I2C1_SR1 & (1U <<2))); //Bit 2 BTF: Byte transfer finished

	I2C1_CR1 |= (1U << 8);
	while (!( I2C1_SR1 & (1U <<0)));
	I2C1_DR = (adress << 1) | 1 ;
	while (!(I2C1_SR1 & (1U <<1)));
	I2C1_CR1 &= ~(1 << 10); // Bit 10 ACK: Acknowledge enable
	(void)I2C1_SR1;
	(void)I2C1_SR2;
	I2C1_CR1 |= (1 << 9); //Bit 9 STOP: Stop generation
	while (!( I2C1_SR1 & (1U <<6))); //Bit 6 RxNE: Data register not empty (receivers)
	while (I2C1_CR1 & (1U << 9));   // wait untill stop finishes
	return I2C1_DR;
}

void I2C_write(uint8_t reg, uint8_t value, uint8_t adress) {
	I2C1_CR1 |= (1U << 8);
	while (!( I2C1_SR1 & (1U <<0)));
	I2C1_DR = (adress << 1) | 0 ;
	while (!(I2C1_SR1 & (1U <<1)));
	(void)I2C1_SR1;
	(void)I2C1_SR2;
	I2C1_DR = reg;
	while (!(I2C1_SR1 & (1U <<7))); //Bit 7 TxE: Data register empty (transmitters)
	I2C1_DR = value;
	while (!(I2C1_SR1 & (1U <<2)));
	I2C1_CR1 |= (1 << 9); //Bit 9 STOP: Stop generation
	while (I2C1_CR1 & (1U << 9));   // wait untill stop finishes
}


int main(void){


	//USART 2 TX,RX
	RCC_APB1ENR &= ~(1U << 17);
	RCC_APB1ENR |= (1U << 17);

	RCC_AHB1ENR &= ~(1U << 0);
	RCC_AHB1ENR |= (1U << 0);

	GPIOA_MODER &= ~(15U << 4);
	GPIOA_MODER |= (2U << 4) | (2U << 6);

	GPIOA_AFRL &= ~((15U << 8) | (15U << 12));
	GPIOA_AFRL |= (7U << 8) | (7U << 12);

	//TX,RX enable
	USART2_CR1 &= ~((1U << 2) |(1U << 3));
	USART2_CR1 |= ((1U << 2) |(1U << 3));

	USART2_BRR = 1667;

	USART2_CR1 &= ~(1U << 13);
	USART2_CR1 |= (1U << 13);

	RCC_AHB1ENR &= ~(1U << 1);
	RCC_AHB1ENR |= (1U << 1);

	RCC_APB1ENR &= ~(1U << 21);
	RCC_APB1ENR |= (1U << 21);

	GPIOB_MODER &= ~((3U << 8*2) | (3U << 9*2));
	GPIOB_MODER |= (2U << 8*2) | (2U << 9*2);

	GPIOB_AFRH &= ~((15U << 0*4) | (15U << 1*4));
	GPIOB_AFRH |= (4U << 0*4) | (4U << 1*4);

	GPIOB_OTYPER &= ~((1U << 8) | (1U << 9));
	GPIOB_OTYPER |= (1U << 8) | (1U << 9);

	I2C1_CR2 = 16;
	I2C1_CCR = 80;
	I2C1_TRISE = 17;


	I2C1_CR1 |= (1U << 0);

	for (volatile int k = 0; k < 200000; k++);

	I2C_write(0x6B, 0x00, 0x68);   // PWR_MGMT_1 = 0 close sleep mode

	while(1){

		uint8_t h = I2C_read(0x3F, 0x68);
		uint8_t l = I2C_read(0x40, 0x68);
		int16_t accel_z = (int16_t)((h << 8) | l);

		int32_t modified_numbers = 0 ;
		int32_t numbers_reversed[11];
		modified_numbers = accel_z;

		if (modified_numbers < 0) {
					while (!(USART2_SR & (1U << 7)));
					USART2_DR = '-';
					modified_numbers = -modified_numbers;
				}

		int i = 0;

		while(1){
			numbers_reversed[i] = (modified_numbers % 10);
			modified_numbers = (modified_numbers / 10);
			i++;

			if (modified_numbers  == 0 ){
				break; }
		}


		for (int t = i; t > 0; t--){
			while (!(USART2_SR & (1U << 7)));//Status register (USART_SR) Address offset: 0x00 Bit 7 TXE: Transmit data register empty Bit 5 RXNE: Read data register not empty

			USART2_DR = numbers_reversed[t-1] + '0';
		}
		while (!(USART2_SR & (1U << 7)));
		USART2_DR = '\n';
		for (volatile int k = 0; k < 1000000; k++);

	}
}
