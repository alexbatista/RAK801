#include "ACLog.h"

uint8_t __isLogEnable = 0;
ACLogLevel __outputLevel[kAmountOfSubSystems];
ACPrint __printFunction = NULL;

const char* ACLogLevel2String(ACLogLevel level) {
    switch (level) {
        case kNoneLevel: return "NONE LEVEL";
        case kInfo: 		 return "INFO";
        case kDebug: 		 return "DEBUG";
        case kWarning: 	 return "WARNING";
        case kError: 		 return "ERROR";
        case kCritical:  return "CRITICAL";
        default: 				 return "";
    }
}

const char* ACLogSubSystem2String(ACLogSubSystem subsystem) {
	switch (subsystem) {
        case kTranslator: return "TRANSLATOR";
				case kLayout:  		return "LAYOUT";
        default: 				  return "";
    }
}

void ACLogInit(ACPrint printFunction) {
    if (printFunction != NULL)
        __printFunction = printFunction;

    for (int i = 0; i < kAmountOfSubSystems; i++) {
        __outputLevel[i] = kNoneLevel;
    }
}

void ACLog(ACLogSubSystem subsystem, ACLogLevel level, const char *message) {
    if (__printFunction == NULL) return;
    static char outputMessage[MESSAGE_MAX_SIZE];

		sprintf(outputMessage, "[%s] %s - %s", ACLogLevel2String(level), ACLogSubSystem2String(subsystem), message);

    if (level >= __outputLevel[subsystem] && __outputLevel[subsystem] != kNoneLevel && __isLogEnable)
        __printFunction(outputMessage);
}

void ACLogSetOutputLevel(ACLogSubSystem subsystem, ACLogLevel level) {
    __outputLevel[subsystem] = level;
}

void ACLogGlobalOn() {
    __isLogEnable = 1;
}

void ACLogGlobalOff() {
    __isLogEnable = 0;
}
