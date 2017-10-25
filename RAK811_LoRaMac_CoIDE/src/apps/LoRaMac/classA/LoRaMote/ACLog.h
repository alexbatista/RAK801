#ifndef ACLOG_H
#define ACLOG_H

#include "ACType.h"

#define MESSAGE_MAX_SIZE 64+256

typedef void (*ACPrint)(const char*);

typedef enum {
    kTranslator = 0,
		kLayout,
    kAmountOfSubSystems
} ACLogSubSystem;

typedef enum {
    kNoneLevel = 0,
    kDebug,
    kInfo,
    kWarning,
    kError,
    kCritical
} ACLogLevel;

void ACLogInit(ACPrint printFunction);
void ACLog(ACLogSubSystem subsystem, ACLogLevel level, const char *message);
void ACLogSetOutputLevel(ACLogSubSystem subsystem, ACLogLevel level);
void ACLogGlobalOn(void);
void ACLogGlobalOff(void);

#endif //ACLOG_H
