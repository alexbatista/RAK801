#ifndef ACIOMAP_H
#define ACIOMAP_H

#include "ACType.h"

//*
#define GPIO_PIN_15 1
#define GPIO_PIN_14 2
#define GPIO_PIN_11 3
#define GPIO_PIN_13 4

#define GPIO_PIN_9  5
#define GPIO_PIN_10 6
#define GPIO_PIN_8  7
//*/

#define DISPLAY_DC   GPIO_PIN_15
#define DISPLAY_RST  GPIO_PIN_14
#define DISPLAY_BS   GPIO_PIN_11
#define DISPLAY_BUSY GPIO_PIN_13

#define SPI_SCLK   GPIO_PIN_9
#define SPI_MOSI   GPIO_PIN_10
#define SPI_MISO   0
#define SPI_N_CS_1 GPIO_PIN_8

static void ACSetGPIO(uint16_t gpio) {
    switch (gpio) {
		case GPIO_PIN_8:
		case GPIO_PIN_9:
        case GPIO_PIN_10:
			//HAL_GPIO_WritePin(GPIOD, gpio, GPIO_PIN_SET);
            break;
        case GPIO_PIN_15:
        case GPIO_PIN_14:
        case GPIO_PIN_13:
        case GPIO_PIN_11:
			//HAL_GPIO_WritePin(GPIOE, gpio, GPIO_PIN_SET);
            break;
		default:
			break;
    }
}

static void ACCClearGPIO(uint16_t gpio) {
    switch (gpio) {
		case GPIO_PIN_8:
		case GPIO_PIN_9:
        case GPIO_PIN_10:
			//HAL_GPIO_WritePin(GPIOD, gpio, GPIO_PIN_RESET);
            break;
        case GPIO_PIN_15:
        case GPIO_PIN_14:
        case GPIO_PIN_13:
        case GPIO_PIN_11:
			//HAL_GPIO_WritePin(GPIOE, gpio, GPIO_PIN_RESET);
            break;
		default:
			break;
    }
}

static uint8_t ACReadGPIO(uint16_t gpio) {
    switch (gpio) {
        case GPIO_PIN_10:
        case GPIO_PIN_9:
        case GPIO_PIN_8:
			return 0;//HAL_GPIO_ReadPin(GPIOD, gpio);
        case GPIO_PIN_15:
        case GPIO_PIN_14:
        case GPIO_PIN_13:
        case GPIO_PIN_11:
			return 0;//HAL_GPIO_ReadPin(GPIOE, gpio);
		default:
			return 0xEE;
    }
}

static void ACDelayMS(uint64_t xms) {
	//HAL_Delay(xms);
}

static void ACDelayUS(uint16_t microseconds) {
    for(;microseconds>1;microseconds--);
}

static void ACDelayNS(uint16_t nanoseconds) {

}

#endif //ACIOMAP_H
