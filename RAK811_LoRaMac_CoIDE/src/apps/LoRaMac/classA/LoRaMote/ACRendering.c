#include "ACRendering.h"

static const GFXfont *_font = NULL;
static ACVector2 _cursor = {0, 0};
static ACDisplayColor _color = kBlack;

//private functions

void ACRenderingDrawChar(unsigned char c) {
		GFXglyph glyph;
		uint8_t  *bitmap;
		uint16_t bo;
		uint8_t  w, h;
		uint8_t  xx, yy, bits = 0, bit = 0;
		ACVector2 pos;
	
		c -= _font->first;
    glyph  = _font->glyph[c];
    bitmap = _font->bitmap;

    bo = glyph.bitmapOffset;
    w  = glyph.width;
    h  = glyph.height;

    for(yy=0; yy<h; yy++) {
        for(xx=0; xx<w; xx++) {
            if(!(bit++ & 7)) {
                bits = bitmap[bo++];
            }
            if(bits & 0x80) {
                pos.x = _cursor.x+xx;
                pos.y = _cursor.y+yy;
				ACDisplaySetPixel(pos.x, pos.y, _color);
            }
            bits <<= 1;
        }
    }
}

void ACRenderingWriteChar(unsigned char c) {
		int16_t old_y;
	
		if(c == '\n') {
        _cursor.x = 0;
        _cursor.y += _font->yAdvance;
    } else if(c != '\r') {
        uint8_t first = _font->first;
        if((c >= first) && (c <= _font->last)) {
            GFXglyph glyph = _font->glyph[c - first];
            uint8_t   w     = glyph.width,
                      h     = glyph.height;
            if((w > 0) && (h > 0)) { // Is there an associated bitmap?
                int16_t xo = glyph.xOffset; // sic
                if((_cursor.x + (xo + w)) > DISPLAY_WIDTH) {
                    _cursor.x  = 0;
                    _cursor.y += _font->yAdvance;
                }
                old_y = _cursor.y;
                _cursor.y += glyph.yOffset;
                ACRenderingDrawChar(c);
                _cursor.y = old_y;
            }
            _cursor.x += glyph.xAdvance;
        }
    }
}

//gets and setters
void ACRenderingSetFont(const GFXfont *font) {
    _font = font;
}

void ACRenderingSetColor(ACDisplayColor color) {
    _color = color;
}

void ACRenderingSetCursor(ACVector2 point) {
    _cursor = point;
}

ACVector2 ACRenderingGetCursor() {
    return _cursor;
}

//primitive functions
void ACRenderingWriteText(const char* text) {
		uint32_t i;
		for (i = 0; i < strlen(text); i++) {
        ACRenderingWriteChar(text[i]);
    }
}

void ACRenderingPaint(ACVector2 position, ACVector2 size) {
    int i, j;
		
		for (i = position.y; i < position.y+size.y; i++) {
        for (j = position.x; j < position.x+size.x; j++) {
            ACDisplaySetPixel(j, i, _color);
        }
    }
}

void ACRenderingDrawCircle(ACVector2 center, uint8_t radius) {
		int x = radius;
    int y = 0;
    int xChange = 1 - (radius << 1);
    int yChange = 0;
    int radiusError = 0;

    while (x >= y)
    {
        for (int i = center.x - x; i <= center.x + x; i++)
        {
						ACDisplaySetPixel(i, center.y + y, _color);
						ACDisplaySetPixel(i, center.y - y, _color);
        }
        for (int i = center.x - y; i <= center.x + y; i++)
        {
						ACDisplaySetPixel(i, center.x + x, _color);
						ACDisplaySetPixel(i, center.x - x, _color);
        }

        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0)
        {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
}

void ACRenderingDrawHorizontalDotted(ACVector2 position, uint16_t width) {
		int i;
		ACDisplayColor color = kWhite;
    for (i = position.x; i < width; i++) {
        if ((i&3) == 0) {
            if (color == kWhite)
                color = _color;
            else
                color = kWhite;
        }
        ACDisplaySetPixel(i, position.y, color);
    }
}

//TODO
void ACRenderingDrawElement(ACTemplateElement *element) {
	
}
