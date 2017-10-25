#ifndef ACUART_H
#define ACUART_H

#include "ACType.h"
#include "ACIOMap.h"

uint8_t ACUARTOpen(void);
uint8_t ACUARTRead(void);
void ACUARTWrite(uint8_t data);
uint8_t ACUARTAvailable(void);

#endif //ACUART_H
