#include "ACUART.h"

uint8_t ACUARTOpen(void) {
	return 1;
}

uint8_t ACUARTRead(void) {
	return 1;
}

void ACUARTWrite(uint8_t data) {
	//HAL_UART_Transmit(&huart1, &data, 1, 1000);
}

uint8_t ACUARTAvailable(void) {
	return 1;
}
