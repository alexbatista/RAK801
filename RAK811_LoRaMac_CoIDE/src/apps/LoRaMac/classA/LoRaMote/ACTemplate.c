#include "ACTemplate.h"

static ACTemplateElement __templates[MAX_NUM_TEMPLATES][MAX_OBJECTS];
static uint8_t __currentTemplate;
static uint8_t __templateSize[MAX_NUM_TEMPLATES];

void ACTemplateInit() {
	__currentTemplate = 0;
	__templateSize[0] = 4;
	
	//define default template
	//top red rectangle
	__templates[0][0].type = kRectangle;
	__templates[0][0].rectangle.position.x = 0;
	__templates[0][0].rectangle.position.y = 0;
	__templates[0][0].rectangle.width = DISPLAY_WIDTH;
	__templates[0][0].rectangle.height = 40;
	__templates[0][0].rectangle.color = kRed;
	
	//name
	__templates[0][1].type = kDynamicText;
	__templates[0][1].dynamicText.position.x = 10;
	__templates[0][1].dynamicText.position.y = 25;
	__templates[0][1].dynamicText.fontSize = 12;
	__templates[0][1].dynamicText.color = kWhite;
	__templates[0][1].dynamicText.placeholder = 1;
	memset(__templates[0][1].dynamicText.text, '\0', 1);
	
	//description
	__templates[0][2].type = kDynamicText;
	__templates[0][2].dynamicText.position.x = 10;
	__templates[0][2].dynamicText.position.y = 65;
	__templates[0][2].dynamicText.fontSize = 12;
	__templates[0][2].dynamicText.color = kBlack;
	__templates[0][2].dynamicText.placeholder = 2;
	memset(__templates[0][2].dynamicText.text, '\0', 1);
	
	//price
	__templates[0][3].type = kDynamicText;
	__templates[0][3].dynamicText.position.x = 172;
	__templates[0][3].dynamicText.position.y = 105;
	__templates[0][3].dynamicText.fontSize = 18;
	__templates[0][3].dynamicText.color = kBlack;
	__templates[0][3].dynamicText.placeholder = 3;
	memset(__templates[0][1].dynamicText.text, '\0', 1);
}

void ACTemplateDraw() {
	for (uint8_t i = 0; i < __templateSize[__currentTemplate]; ++i) {
		ACRenderingDrawElement(&__templates[__currentTemplate][i]);
	}
}

ACOutputStatus ACTemplateUpdateContent(ACMessage *message) {
	uint8_t totalSize = message->fields.size;
	int16_t watchdog = MAX_CONTENT_SIZE;
	uint16_t start = 0;
	
	while (totalSize || watchdog > 0) {
		uint8_t placeholder = message->fields.content[0+start];
		uint8_t size = message->fields.content[1+start];
		
		for (uint8_t i = 0; i < __templateSize[__currentTemplate]; ++i) {
			if (__templates[__currentTemplate][i].type != kDynamicText) continue;
			if (__templates[__currentTemplate][i].dynamicText.placeholder != placeholder) continue;
			
			memcpy(__templates[__currentTemplate][i].dynamicText.text, message->fields.content+start+2, size);
			break;
		}
		
		totalSize -= size;
		watchdog -= size;
		start += size;
	}
	
	//if (watchdog >= 0)
	//	ACTemplateDraw();
	
	return watchdog >= 0 ? kOk : kFail;
}

void ACTemplateGetContent(uint8_t *content, uint16_t *size) {
	uint8_t placeholder;
	uint8_t textLength;
	char *text;
	
	*size = 0;
	
	for (uint8_t i = 0; i < __templateSize[__currentTemplate]; ++i) {
		if (__templates[__currentTemplate][i].type != kDynamicText) continue;
		
		placeholder = __templates[__currentTemplate][i].dynamicText.placeholder;
		textLength = strlen(__templates[__currentTemplate][i].dynamicText.text);
		text = __templates[__currentTemplate][i].dynamicText.text;
		
		memset(content+(*size), placeholder, 1);
		memset(content+(*size)+1, textLength, 1);
		memcpy(content+(*size)+2, text, textLength);
		
		*size += textLength;
	}
}

void ACTemplateGetCurrentTemplate(uint8_t *currentTemplate, uint16_t *size) {
	*size = 0;
	
	for (uint8_t i = 0; i < __templateSize[__currentTemplate]; ++i) {
		
		memset(currentTemplate+(*size), __templates[__currentTemplate][i].type, 1);
		
		switch (__templates[__currentTemplate][i].type) {
			case kCircle:
				memcpy(currentTemplate+(*size)+1, &__templates[__currentTemplate][i].circle.center.x, 2);
				memcpy(currentTemplate+(*size)+3, &__templates[__currentTemplate][i].circle.center.y, 2);
				memset(currentTemplate+(*size)+5, __templates[__currentTemplate][i].circle.radius, 1);
				memset(currentTemplate+(*size)+6, __templates[__currentTemplate][i].circle.color, 1);
			
				*size += 7;
			
				break;
			case kRectangle:
				memcpy(currentTemplate+(*size)+1, &__templates[__currentTemplate][i].rectangle.position.x, 2);
				memcpy(currentTemplate+(*size)+3, &__templates[__currentTemplate][i].rectangle.position.y, 2);
				memcpy(currentTemplate+(*size)+5, &__templates[__currentTemplate][i].rectangle.width, 2);
				memcpy(currentTemplate+(*size)+7, &__templates[__currentTemplate][i].rectangle.height, 2);
				memset(currentTemplate+(*size)+9, __templates[__currentTemplate][i].rectangle.color, 1);
				
				*size += 10;
			
				break;
			case kStaticText:
				memcpy(currentTemplate+(*size)+1, &__templates[__currentTemplate][i].staticText.position.x, 2);
				memcpy(currentTemplate+(*size)+3, &__templates[__currentTemplate][i].staticText.position.y, 2);
				memset(currentTemplate+(*size)+5, __templates[__currentTemplate][i].staticText.fontSize, 1);
				memset(currentTemplate+(*size)+6, __templates[__currentTemplate][i].staticText.color, 1);
				
				*size += 7;
			
				break;
			case kDynamicText:
				memcpy(currentTemplate+(*size)+1, &__templates[__currentTemplate][i].dynamicText.position.x, 2);
				memcpy(currentTemplate+(*size)+3, &__templates[__currentTemplate][i].dynamicText.position.y, 2);
				memset(currentTemplate+(*size)+5, __templates[__currentTemplate][i].dynamicText.fontSize, 1);
				memset(currentTemplate+(*size)+6, __templates[__currentTemplate][i].dynamicText.color, 1);
				memset(currentTemplate+(*size)+7, __templates[__currentTemplate][i].dynamicText.placeholder, 1);
			
				*size += 8;
			
				break;
			case kBarcode:
				memcpy(currentTemplate+(*size)+1, &__templates[__currentTemplate][i].barcode.position.x, 2);
				memcpy(currentTemplate+(*size)+3, &__templates[__currentTemplate][i].barcode.position.y, 2);
				memcpy(currentTemplate+(*size)+5, &__templates[__currentTemplate][i].barcode.code, 13);
				
				*size += 18;
			
				break;
			case kQRCode:
				memcpy(currentTemplate+(*size)+1, &__templates[__currentTemplate][i].qrCode.position.x, 2);
				memcpy(currentTemplate+(*size)+3, &__templates[__currentTemplate][i].qrCode.position.y, 2);
				
				*size += 5;
			
				break;
			default:
				break;
		}
	}
}
