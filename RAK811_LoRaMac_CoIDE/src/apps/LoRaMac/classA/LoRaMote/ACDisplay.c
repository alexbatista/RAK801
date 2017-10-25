#include "ACDisplay.h"

static uint8_t buffer_black[DISPLAY_WIDTH][DISPLAY_HEIGHT >> 3] = {0};
static uint8_t buffer_red[DISPLAY_WIDTH][DISPLAY_HEIGHT >> 3] = {0};
static ACDriver *_driver;
static uint8_t __locked;

uint8_t ACDisplayGetBitFromArrayWithTwoIntegers(uint8_t array[DISPLAY_WIDTH][DISPLAY_HEIGHT>>3], uint16_t x, uint16_t y) {
    return bit( array[x][((DISPLAY_HEIGHT - 1) - y) >> 3], (y & 7) + 1 );
}

void ACDisplaySetBitToArrayWithTwoIntegers(uint8_t array[DISPLAY_WIDTH][DISPLAY_HEIGHT>>3], uint16_t x, uint16_t y, uint8_t bit) {
    if (bit != 0)
        array[x][((DISPLAY_HEIGHT - 1) - y) >> 3] |= 1 << (y & 7);
    else
        array[x][((DISPLAY_HEIGHT - 1) - y) >> 3] &= ~(1 << (y & 7));
}

void ACDisplayInit(ACDriver *driver) {
    _driver = driver;
}

void ACDisplayWriteCommand(uint8_t byte) {
		_driver->open(DISPLAY_ADDR);
    ACCClearGPIO(DISPLAY_DC);
    _driver->write(byte);
		_driver->close();
}

void ACDisplayWriteData(uint8_t byte) {
		_driver->open(DISPLAY_ADDR);
		ACSetGPIO(DISPLAY_DC);
    _driver->write(byte);
		_driver->close();
}

void ACDisplaySetPixel(uint16_t x, uint16_t y, ACDisplayColor color) {
    switch (color) {
        case kWhite:
            ACDisplaySetBitToArrayWithTwoIntegers(buffer_black, x, y, NO_COLOR);
            ACDisplaySetBitToArrayWithTwoIntegers(buffer_red,   x, y, NO_COLOR);
            break;
        case kBlack:
            ACDisplaySetBitToArrayWithTwoIntegers(buffer_black, x, y, WITH_COLOR);
            break;
        case kRed:
            ACDisplaySetBitToArrayWithTwoIntegers(buffer_red,   x, y, WITH_COLOR);
            break;
        default:
            break;
    }
}

ACDisplayColor ACDisplayGetPixel(uint16_t x, uint16_t y) {
    if (ACDisplayGetBitFromArrayWithTwoIntegers(buffer_black, x, y) != 0)
        return kBlack;
    else if (ACDisplayGetBitFromArrayWithTwoIntegers(buffer_red, x, y) != 0)
        return kRed;
    else
        return kWhite;
}

void ACDisplayWaitBusyFlag() {
    while(1)
			if(ACReadGPIO(DISPLAY_BUSY) != 0) break;
    ACDelayMS(20);
}

void ACDisplayUpdate() {
		int i, j;
		
		if (_driver == NULL) return;
	
		__locked = 1;
	
		ACCClearGPIO(DISPLAY_BS);
		ACCClearGPIO(DISPLAY_RST);
		ACDelayMS(10);
		ACSetGPIO(DISPLAY_RST);
		ACDelayMS(10);
	
    //Booster soft start
    ACDisplayWriteCommand(0x06);
    ACDisplayWriteData(0x17);
    ACDisplayWriteData(0x17);
    ACDisplayWriteData(0x17);
    //Power on
    ACDisplayWriteCommand(0x04);
    //Check busy_flag
    ACDisplayWaitBusyFlag();
    //Panel setting
    ACDisplayWriteCommand(0x00);
    ACDisplayWriteData(0x8F);
    //Vcom and data interval setting
    ACDisplayWriteCommand(0x50);
    ACDisplayWriteData(0x77);
    //Resolution setting
    ACDisplayWriteCommand(0x61);
    ACDisplayWriteData(0x80);
    ACDisplayWriteData(0x01);
    ACDisplayWriteData(0x28);
    //Display start transmission
    ACDisplayWriteCommand(0x10);
    //Transport black/white data
    for (i = 0; i < DISPLAY_WIDTH; i++) {
        for (j = 0; j < DISPLAY_HEIGHT >> 3; j++) {
            ACDisplayWriteData(~buffer_black[i][j]);
        }
    }
		ACDelayMS(2);
    //Display start transmission
    ACDisplayWriteCommand(0x13);
    //Transport red data
    for (i = 0; i < DISPLAY_WIDTH; i++) {
        for (j = 0; j < DISPLAY_HEIGHT >> 3; j++) {
            ACDisplayWriteData(~buffer_red[i][j]);
        }
    }
		ACDelayMS(2);
    //Display refresh
    ACDisplayWriteCommand(0x12);
    //Check busy_flag
    ACDisplayWaitBusyFlag();
    ACDelayMS(100);
    //Power OFF
    ACDisplayWriteCommand(0x02);
    ACDisplayWaitBusyFlag();
    //Deep sleep
    ACDisplayWriteCommand(0x07);
    ACDisplayWriteData(0xA5);
    ACDelayMS(40);
		
		__locked = 0;
}

void ACDisplayClear() {
		int i, j;
	
    for (i = 0; i < DISPLAY_WIDTH; i++) {
        for (j = 0; j < DISPLAY_HEIGHT >> 3; j++) {
            buffer_black[i][j] = 0;
            buffer_red[i][j]   = 0;
        }
    }
}

uint8_t ACDisplayIsLocked(void) {
	return __locked;
}
