#ifndef ACDRIVER_H
#define ACDRIVER_H

#include "ACType.h"

typedef enum {
    kNone,
    kAddressNotFound,
    kCannotSendMessage,
    kNotOpen,
    kNullDriver
} ACDriverError;

typedef void (*ACDriverOpen)(uint8_t);
typedef void (*ACDriverClose)();
typedef ACDriverError (*ACDriverWrite)(uint8_t);
typedef ACDriverError (*ACDriverRead)(uint8_t*);

typedef struct {
    ACDriverOpen  open;
    ACDriverClose close;
    ACDriverWrite write;
    ACDriverRead  read;
} ACDriver;

#endif //ACDRIVER_H
