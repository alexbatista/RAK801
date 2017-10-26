#ifndef ACTEMPLATE_H
#define ACTEMPLATE_H

#include "ACType.h"
#include "ACTranslator.h"
#include "ACRendering.h"
#include "ACLog.h"

#define MAX_OBJECTS 	  7
#define MAX_NUM_TEMPLATES 4

void ACTemplateInit();
ACOutputStatus ACTemplateUpdateContent(ACMessage *message);
void ACTemplateGetContent(uint8_t *content, uint16_t *size);
void ACTemplateGetCurrentTemplate(uint8_t *currentTemplate, uint16_t *size);

#endif //ACTEMPLATE_H
