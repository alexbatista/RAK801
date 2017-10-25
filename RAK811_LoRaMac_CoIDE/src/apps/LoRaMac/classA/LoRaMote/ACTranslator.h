#ifndef ACTRANSLATOR_H
#define ACTRANSLATOR_H

#include "ACLog.h"

#define START_CODE   0x01
#define END_CODE 	 0x04

#define SIZE_OF_HEADER 	   6
#define SIZE_MESSAGE_CACHE 3

#define MAX_PACKET_SIZE 	   100
#define MAX_NUMBERS_OF_PACKETS 5
#define MAX_CONTENT_SIZE 	   MAX_NUMBERS_OF_PACKETS * MAX_PACKET_SIZE

#define ACMessageTypeExist(type) ((type) == kUploadTemplate ||\
																	(type) == kContent 				||\
																	(type) == kConfig 				||\
																	(type) == kChangeTemplate ||\
																	(type) == kFirmware)

typedef enum {
	kUploadTemplate = 0,
	kContent,
	kConfig,
	kChangeTemplate,
	kFirmware,
	kAmountOfMessageTypes
} ACMessageType;

typedef union {
	uint8_t total[6+MAX_CONTENT_SIZE];
	struct {
		int id:16;
		int packetCounter:4;
		int maxPackets:4;
		int type:8;
		int size:16;
		uint8_t content[MAX_CONTENT_SIZE];
	} fields;
} ACMessage;

typedef ACOutputStatus (*ACTranslatorCallback)(ACMessage*);

void ACTranslatorInit(ACTranslatorCallback receiveCallbacks[kAmountOfMessageTypes]);
ACOutputStatus ACTranslatorHandleReciveMessage(uint8_t *buffer, uint8_t bufferSize);
ACMessage* ACTranslatorGetLastReceivedMessage();

#endif //ACTRANSLATOR_H
