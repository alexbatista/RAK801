#ifndef ACRENDERING_H
#define ACRENDERING_H

#include "FreeSansBold12pt7b.h"
#include "FreeSansBold18pt7b.h"
#include "FreeSansBold24pt7b.h"
#include "FreeSerif12pt7b.h"
#include "FreeMono12pt7b.h"

#include "gfxfont.h"
#include "ACDisplay.h"
//Tenis Nike Air Max Motion Masc.
#define MAX_TEXT_LENGTH   35

typedef enum PACKED {
	kCircle = 1,
	kRectangle,
	kStaticText,
	kDynamicText,
	kBarcode,
	kQRCode
} ACDrawableElementType;

#define ACDrawableElementExist(element) ((element) == kCircle 	   ||\
										 (element) == kRectangle   ||\
										 (element) == kStaticText  ||\
										 (element) == kDynamicText ||\
										 (element) == kBarcode 	   ||\
										 (element) == kQRCode)

typedef union PACKED {
	uint8_t total[7+MAX_TEXT_LENGTH];
	uint8_t type;
	struct PACKED {
		uint8_t  type;
		ACVector2 center;
		uint8_t  radius;
		uint8_t color;
	} circle;
	struct PACKED {
		uint8_t  type;
		ACVector2 position;
		uint16_t width;
		uint16_t height;
		uint8_t  color;
	} rectangle;
	struct PACKED {
		uint8_t  type;
		ACVector2 position;
		uint8_t  fontSize;
		uint8_t  color;
		char text[MAX_TEXT_LENGTH];
	} staticText;
	struct PACKED {
		uint8_t  type;
		ACVector2 position;
		uint8_t  fontSize;
		uint8_t  color;
		uint8_t  placeholder;
		char text[MAX_TEXT_LENGTH];
	} dynamicText;
	struct PACKED {
		uint8_t type;
		ACVector2 position;
		uint8_t code[13];
	} barcode;
	struct PACKED {
		uint8_t type;
		ACVector2 position;
		//uint32_t code;
	} qrCode;
} ACTemplateElement;

//public functions
void ACRenderingDrawElement(ACTemplateElement *element);

//gets and setters
void ACRenderingSetFont(const GFXfont *font);
void ACRenderingSetColor(ACDisplayColor color);
void ACRenderingSetCursor(ACVector2 pos);
ACVector2 ACRenderingGetCursor(void);

//primitive functions
void ACRenderingWriteText(const char *text);
void ACRenderingPaint(ACVector2 position, ACVector2 size);
void ACRenderingDrawCircle(ACVector2 center, uint8_t radius);
void ACRenderingDrawHorizontalDotted(ACVector2 position, uint16_t width);

#endif //ACRENDERING_H
