#ifndef ACDISPLAY_H
#define ACDISPLAY_H

#include <string.h>
#include "ACType.h"
#include "ACDriver.h"
#include "ACIOMap.h"

#define DISPLAY_WIDTH  296
#define DISPLAY_HEIGHT 128
#define DISPLAY_ADDR   1

#define NO_COLOR   0
#define WITH_COLOR 1

typedef enum {
    kWhite = 0,
    kBlack,
    kRed
} ACDisplayColor;

void ACDisplayInit(ACDriver *driver);

void ACDisplayWriteCommand(uint8_t byte);
void ACDisplayWriteData(uint8_t byte);

void ACDisplaySetPixel(uint16_t x, uint16_t y, ACDisplayColor color);
ACDisplayColor ACDisplayGetPixel(uint16_t x, uint16_t y);
void ACDisplayUpdate(void);
void ACDisplayClear(void);
uint8_t ACDisplayIsLocked(void);

#endif //ACDISPLAY_H
