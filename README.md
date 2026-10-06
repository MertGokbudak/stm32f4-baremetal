# STM32F4 Bare-Metal Projects

Register-level projects for STM32F4, written without HAL.
The goal is to learn the peripherals from the reference manual and gradually
build some projects.

<img width="500" alt="Hardware setup" src="https://github.com/user-attachments/assets/f6c581d6-e29e-4f42-b6b2-9bd45e0fbe01" />

*Bottom: Nucleo-F446RE with joystick, HC-12 (transmitter) and MPU6050. Top: Black Pill with HC-12 (receiver) and servo, powered by a power bank.*

Boards:
- NUCLEO-F446RE
- WeAct Black Pill (STM32F411CE)

All projects run on the default 16 MHz HSI clock. UART baud rate: 9600.

## Projects

### Wireless joystick-controlled servo (HC-12)
Two boards communicate over a pair of HC-12 433 MHz wireless serial modules.
The Nucleo reads a joystick and sends its position; the Black Pill receives it
and moves a servo. Both boards also drive a local servo / debug output.

#### nucleo_joystick_servo_hc_12 (NUCLEO-F446RE) — transmitter
- PA0: joystick X axis (ADC1 channel 0)
- PA6: local servo PWM (TIM3 CH1, 50 Hz)
- PA2 / PA3: USART2, prints the joystick value as text (ST-Link virtual COM port)
- PA9: USART1 TX → HC-12 RXD, sends the 8-bit joystick value

#### blackpill_uart_servo_hc_12 (Black Pill F411CE) — receiver
- PA10: USART1 RX ← HC-12 TXD
- PA6: servo PWM (TIM3 CH1, 50 Hz), position mapped from the received byte

### nucleo_mpu6050 (NUCLEO-F446RE)
Reads the Z-axis acceleration from an MPU6050 over I2C and prints it over UART.
- PB8: I2C1 SCL
- PB9: I2C1 SDA
- MPU6050 address: 0x68
- PA2: USART2 TX (ST-Link virtual COM port)
