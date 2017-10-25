#ifndef ACTYPE_H
#define ACTYPE_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define bit(byte, pos) (((0x01 << ((pos)-1)) & (byte)) >> ((pos)-1))
#define AC_ENABLE 1
#define AC_DISABLE 0

typedef enum {
    kFail = 0,
    kOk
} ACOutputStatus;

typedef enum {
    kLow = 0,
    kHigh
} ACPinLevel;

typedef enum {
    kFalse = 0,
    kTrue
} ACBool;

typedef struct __attribute__((packed)) {
    uint16_t x;
    uint8_t y;
} ACVector2;

#endif //ACTYPE_H
