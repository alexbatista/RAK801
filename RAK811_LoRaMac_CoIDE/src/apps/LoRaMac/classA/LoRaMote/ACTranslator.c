#include "ACTranslator.h"

static ACTranslatorCallback __receiveCallbacks[kAmountOfMessageTypes];
static ACMessage __messageCache[SIZE_MESSAGE_CACHE];
static uint8_t __messageCacheIndex;
static uint16_t __lastReceivedMessageId;

void ACTranslatorInit(ACTranslatorCallback callbacks[kAmountOfMessageTypes]) {
	memcpy(__receiveCallbacks, callbacks, sizeof(ACTranslatorCallback)*kAmountOfMessageTypes);
	
	__messageCacheIndex = 0;
	__lastReceivedMessageId = 0;
	
	for (uint8_t i = 0; i < SIZE_MESSAGE_CACHE; ++i) {
		memset(__messageCache[i].total, 0, 6+MAX_CONTENT_SIZE);
	}
}

uint8_t ACTranslatorReverseByte(uint8_t byte) {
	byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4;
	byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2;
	byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1;
	
	return byte;
}

ACMessage* ACTranslatorGetMessageInCache(uint16_t id) {
	uint8_t index;
	//search in cache if exist a message with this id
	for (index = 0; index < SIZE_MESSAGE_CACHE; ++index) {
		if (__messageCache[index].fields.id == id) {
			break;
		}				
	}
	//if not exist, add this message to cache
	if (index == SIZE_MESSAGE_CACHE) {
		index = __messageCacheIndex;
		//increment index of cache, in circular buffer mode
		if (__messageCacheIndex == SIZE_MESSAGE_CACHE - 1) {
			__messageCacheIndex = 0;
		}
		else {
			__messageCacheIndex++;
		}
	}
	
	return &(__messageCache[index]);
}

ACMessage* ACTranslatorGetLastReceivedMessage() {
	return &__messageCache[__lastReceivedMessageId];
}

ACOutputStatus ACTranslatorHandleReciveMessage(uint8_t *buffer, uint8_t bufferSize) {
	static char outputMessage[256];
	
	//check if message size is greater than SIZE_OF_HEADER+1
	if (bufferSize < SIZE_OF_HEADER+1) {
		sprintf(outputMessage, "The minimum size of message is 7. Size = %d\n", bufferSize);
		ACLog(kTranslator, kError, outputMessage);
		return kFail;
	}
	
	//check start code
	if (buffer[0] != START_CODE) {
			sprintf(outputMessage, "Start code wrong. Code = 0x%x\n", buffer[0]);
			ACLog(kTranslator, kError, outputMessage);
			return kFail;
	}
	
	//print start code
	sprintf(outputMessage, "Start code = 0x%x\n", buffer[0]);
	ACLog(kTranslator, kDebug, outputMessage);
	
	//get ID and 
	uint16_t id = buffer[1] | (buffer[2] << 8);
	//get message in cache with this id
	ACMessage *message =  ACTranslatorGetMessageInCache(id);
	//set message id
	message->fields.id = id;
	
	//print message ID
	sprintf(outputMessage, "Message ID = 0x%x\n", id);
	ACLog(kTranslator, kDebug, outputMessage);
	
	//get packet index 
	uint8_t packetIndex = buffer[3] & 0x0F;
	//get max packets
	message->fields.maxPackets = (buffer[3] & 0xF0) >> 4;
	
	//check if the index of packet is less than the max expected
	if (packetIndex >= message->fields.maxPackets) {
		sprintf(outputMessage, "The index of packet is greater than the max expected. PacketIndex/MaxPackets = %d/%d\n", packetIndex, message->fields.maxPackets);
		ACLog(kTranslator, kWarning, outputMessage);
		return kFail;
	}
	
	//print the packet index and max packets		
	sprintf(outputMessage, "PacketIndex/MaxPackets = %d/%d\n", packetIndex, message->fields.maxPackets);
	ACLog(kTranslator, kDebug, outputMessage);
	
	//get message type
	message->fields.type = buffer[4];
		
	//check if the message type exist
	if (!ACMessageTypeExist(message->fields.type)) {
		sprintf(outputMessage, "Message Type does not exist. Type = %d\n", message->fields.type);
		ACLog(kTranslator, kError, outputMessage);
		return kFail;
	}
		
	//print message type
	sprintf(outputMessage, "Message type = %d\n", message->fields.type);
	ACLog(kTranslator, kDebug, outputMessage);
	
	//get size of content
	uint8_t size = buffer[5];
	
	//check if the size inside message is equal to real message size
	if (size + SIZE_OF_HEADER + 1 != bufferSize) {
		sprintf(outputMessage, "The amount of contents is different of the size inside message. Size in message = %d. Real message size = %d\n", size, bufferSize);
		ACLog(kTranslator, kWarning, outputMessage);
		return kFail;
	}
	
	//print content size
	sprintf(outputMessage, "Content Size = %d\n", size);
	ACLog(kTranslator, kDebug, outputMessage);
	
	//get content from buffer
	uint8_t messageContent[256];
	memcpy(messageContent, buffer+SIZE_OF_HEADER, size);
	
	//print content
	for (uint8_t i = 0; i < size; ++i) {
		if (i == 0)
			sprintf(outputMessage, "Content = 0x%x|", buffer[i+SIZE_OF_HEADER]);
		else
			sprintf(outputMessage, "0x%x|", buffer[i+SIZE_OF_HEADER]);
		ACLog(kTranslator, kDebug, outputMessage);
	}
	
	//sorting content
	memcpy(message->fields.content+(MAX_PACKET_SIZE*packetIndex), messageContent, size);
	message->fields.size += size;
	
	//increment the number of packets received
	message->fields.packetCounter++;
	
	//save the id of this message in 'lastReceivedMessageId' variable
	__lastReceivedMessageId = id;
	
	if (message->fields.packetCounter < message->fields.maxPackets)
		return kOk;
	
	//check if callback is NULL
	if (__receiveCallbacks[message->fields.type] == NULL) {
		sprintf(outputMessage, "Callback is NULL. Type = %d\n", message->fields.type);
		ACLog(kTranslator, kCritical, outputMessage);
		return kFail;
	}
	
	//call callback
	return __receiveCallbacks[message->fields.type](message);
}
