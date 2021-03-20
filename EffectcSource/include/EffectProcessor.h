#pragma once
#ifdef __cplusplus 
extern "C"{
#endif

#include "Color_t.h"
#include "EffectStateMachine.h"
#include "EffectWaveforms.h"
#include <stdint.h>

typedef struct EffectProcessor_def {
    uint8_t colorSize;
    Color_t   Color;
    Color_t   ColorOld;
    uint8_t fadeSteps;
    uint8_t fadingCnt;
    EffectSM_t EffPV;
    EffectSM_t EffPV_old;
}EffectProcessor_t;

void EP_Construct(EffectProcessor_t * self, uint16_t const templateLength, uint8_t const fadeSteps);
void EP_SetEffect(EffectProcessor_t * self, EffMacro_t *sequence, Color_t const *sColor, uint8_t const * intens);
Color_t const * EP_Tick(EffectProcessor_t * self);

#ifdef __cplusplus
}
#endif