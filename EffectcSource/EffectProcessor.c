/**
 * @file EffectProcessor.c
 * @author Gustice
 * @brief Implementation of Effect-Processor
 * @version 0.6
 * @date 2019-10-03
 *
 * @copyright Copyright (c) 2019
 */

#include "EffectProcessor.h"
#include "EffectBasics.h"
#include "EffectMacro.h"
#include "EffectWaveforms.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

Color_t *crossFadeColors(EffectProcessor_t *self, uint8_t k) {
    ScaleColor(&self->ColorOld, k);
    ScaleColor(&self->Color, 0xFF-k);
    MixColor(&self->Color, self->ColorOld);

    return &self->Color;
}

void EP_Construct(EffectProcessor_t *self, uint16_t const templateLength, uint8_t const fadeSteps) {
    SM_ParameterValues_t param  = {
        .idleIntens = 0xFF,
        .dynamicRange = 30,
        .fadeSteps = fadeSteps,
        .templateLength = templateLength
    };
    SM_Construct(&self->EffPV, param, &self->Color);
    SM_Construct(&self->EffPV_old, param, &self->ColorOld);
    
    SM_SetEffect(&self->EffPV, eff_Dark, NO_COLOR, &gu8_fullIntensity, 0);
    self->fadeSteps = fadeSteps;
}

void EP_SetEffect(EffectProcessor_t *self, EffMacro_t *sequence, Color_t const *sColor, uint8_t const *intens) {
    self->fadingCnt = self->fadeSteps;
    memcpy(&self->EffPV_old, &self->EffPV, sizeof(EffectSM_t));
    self->EffPV.outputColor = &self->Color;
    self->EffPV_old.outputColor = &self->ColorOld;

    SM_SetEffect(&self->EffPV, sequence, sColor, intens, 0);
}

Color_t const *EP_Tick(EffectProcessor_t *self) {
    uint8_t k;
    SetColor(&self->Color, *SM_Tick(&self->EffPV));

    if (self->fadingCnt > 0) { // allways process soft cross dissolve between different macros
        self->fadingCnt--;
        k = (uint16_t)self->fadingCnt * 0xFF / self->fadeSteps;

        SetColor(&self->ColorOld, *SM_Tick(&self->EffPV_old));
        crossFadeColors(self, k);
    }

    return &self->Color;
}
