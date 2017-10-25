#ifndef ACSPI_H
#define ACSPI_H

#include "ACType.h"
#include "ACDriver.h"
#include "ACIOMap.h"

void ACSPIInit(ACDriver *driver);

void ACSPIOpen(uint8_t addr);
void ACSPIClose(void);
ACDriverError ACSPIWrite(uint8_t data);
ACDriverError ACSPIRead(uint8_t* data);

#endif //ACSPI_H
