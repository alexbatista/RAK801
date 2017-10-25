#include "ACSPI.h"

static uint8_t _addr = 0;

void ACSPIInit(ACDriver *driver) {
    driver->open = ACSPIOpen;
    driver->close = ACSPIClose;
    driver->write = ACSPIWrite;
    driver->read = ACSPIRead;
}

void ACSPIDelay() {
    uint8_t i;
    uint8_t xrate = 1;
	while(xrate)
	{
		for(i=0;i<2;i++);
		xrate--;
	}
}

void ACSPIOpen(uint8_t addr) {
    _addr = addr;
		ACSPIDelay();
		switch (_addr) {
        case 1:
            ACCClearGPIO(SPI_N_CS_1);
            break;
        default:
            break;
    }
}

void ACSPIClose() {
    switch (_addr) {
        case 1:
            ACSetGPIO(SPI_N_CS_1);
            break;
        default:
            break;
    }
}

ACDriverError ACSPIWrite(uint8_t data) {
		int i;
		ACSPIDelay();
    for(i = 0; i < 8; i++)
    {
        ACCClearGPIO(SPI_SCLK);
        ACSPIDelay();
        if (data & 0x80)
            ACSetGPIO(SPI_MOSI);
        else
            ACCClearGPIO(SPI_MOSI);
        data = data << 1;
        ACSPIDelay();
        ACDelayUS(1);
        ACSetGPIO(SPI_SCLK);
        ACSPIDelay();
    }

    return kNone;
}

ACDriverError ACSPIRead(uint8_t* data) {
    uint8_t mask = 0x80;
    uint8_t bit = 0;
		int i;

    for(i = 0; i < 8; i++)
    {
        ACSPIDelay();
        ACCClearGPIO(SPI_SCLK);
        ACSPIDelay();
        ACSPIDelay();
        ACSetGPIO(SPI_SCLK);
        //read
        bit = ACReadGPIO(SPI_MISO);
        if (bit != 0)
            *data |= mask;
        else
            *data &= ~(mask);
        mask = mask >> 1;
        ACSPIDelay();
    }

    return kNone;
}
