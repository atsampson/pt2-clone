#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "pt2_header.h"
#include "pt2_structs.h"

#define SCOPE_FRAC_BITS 16
#define SCOPE_FRAC_SCALE (1UL << 16)
#define SCOPE_FRAC_MASK (SCOPE_FRAC_SCALE-1)

typedef struct scope_t
{
	const int8_t *data;
	bool active, emptyScopeDrawn;
	uint8_t volume;
	int32_t length, pos;
	uint32_t delta, posFrac;

	const int8_t *newData;
	int32_t newLength;
} scope_t;

void scopeTrigger(int32_t ch, int32_t length);
int32_t getSampleReadPos(int32_t ch, uint8_t smpNum);
void updateScopes(void);
void drawScopes(void);
bool initScopes(void);
void stopScope(int32_t ch);
void stopAllScopes(void);

extern scope_t scope[AMIGA_VOICES];
