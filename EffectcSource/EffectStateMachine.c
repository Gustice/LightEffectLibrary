/**
 * @file EffectStateMachine.cpp
 * @author Gustice
 * @brief Implementation of Effect-State-Machine
 * @version 0.1
 * @date 2019-10-01
 *
 * @copyright Copyright (c) 2019
 */

#include "EffectStateMachine.h"
#include "EffectWaveforms.h"
#include <math.h>
#include <stdlib.h>

Color_t const *UpdateBlank(EffectSM_t *self) {
    ScaleColor(self->outputColor, 0);
    return self->outputColor;
}

Color_t const *UpdateIdle(EffectSM_t *self) {
    SetColor(self->outputColor, self->curentColor);
    ScaleColor(self->outputColor, self->SMIParams.idleIntens);
    return self->outputColor;
}

Color_t const *UpdateFreeze(EffectSM_t *self) {
    // just do nothing and Return currently set color
    return self->outputColor;
}

Color_t const *UpdateWave(EffectSM_t *self) {

    EffMacro_t const *const cEffStep = self->p_effMac;
    SetColor(self->outputColor, self->curentColor);
    ScaleColor(self->outputColor, cEffStep->pWave[SM_GetWaveIdx(self)]);
    ScaleColor(self->outputColor, cEffStep->FsIntensity);
    return self->outputColor;
}

Color_t const *UpdateRevWave(EffectSM_t *self) {
    EffMacro_t const *const cEffStep  = self->p_effMac;
    uint8_t                 lastIndex = self->SMIParams.templateLength - 1;
    SetColor(self->outputColor, self->curentColor);
    ScaleColor(self->outputColor, cEffStep->pWave[lastIndex - SM_GetWaveIdx(self)]);
    ScaleColor(self->outputColor, cEffStep->FsIntensity);
    return self->outputColor;
}

Color_t const *UpdateFlicker(EffectSM_t *self) {
    uint8_t k;
    k = rand();
    k = self->SMIParams.idleIntens + ((int16_t)k * (self->SMIParams.dynamicRange) / 0xFF);
    SetColor(self->outputColor, self->curentColor);
    ScaleColor(self->outputColor, k);
    return self->outputColor;
}

static const EffMacro_t delayPrequel[] = {
    {Light_Blank, (uint8_t *)0, 0, 1, &cBlack, 0, 0},
};


typedef Color_t const *pEffPrcHandle(EffectSM_t *self);
/// Function table to processing Functions
pEffPrcHandle *const apF_Processors[6] = {
    UpdateBlank, UpdateIdle, UpdateFreeze, UpdateWave, UpdateRevWave, UpdateFlicker,
};

void SM_Construct(EffectSM_t *self, SM_ParameterValues_t param, Color_t *color) {
    memset(self, 0, sizeof(EffectSM_t));
    memcpy(&(self->SMIParams), &param, sizeof(SM_ParameterValues_t));
    self->outputColor = color;
}

const uint8_t SM_GetWaveIdx(EffectSM_t *self) { return ((self->SMPValues.waveIdx & 0xFF00u) >> 8); };

void SM_SetDynamicRange(EffectSM_t *self, uint8_t range) { self->SMIParams.dynamicRange = range; };

void SetIndexes(EffectSM_t *self) {
    self->SMPValues.waveStep = ((self->SMIParams.templateLength) << 8);
    self->SMPValues.waveStep /= self->p_effMac->duration;
    self->SMPValues.waveIdx = (uint16_t)(0 - self->SMPValues.waveStep);
}

Color_t const *SM_Tick(EffectSM_t *self) {
    // tick-increment
    if (--self->SMPValues.tick == 0) {
        // repeats-decrement
        if (self->SMPValues.repeats-- == 0) {
            self->SMPValues.macroIdx = self->p_effMac->next;
            self->p_effMac           = &(self->p_effSeq[self->p_effMac->next]);
            self->SMPValues.repeats  = self->p_effMac->repeats;

            // execute color change if necessary
            if (self->p_effMac->color != NO_COLOR) {
                self->SMPValues.dissolveCnt = self->SMIParams.fadeSteps;
                SetColor(&self->lastColor, self->curentColor);
                SetColor(&self->curentColor, *self->p_effMac->color);
            }
        }

        self->SMPValues.tick = self->p_effMac->duration;
        SetIndexes(self);
    }

    self->SMPValues.waveIdx += self->SMPValues.waveStep;
    if (self->SMPValues.dissolveCnt > 0) {
        self->SMPValues.dissolveCnt--;
    }

    return apF_Processors[self->p_effMac->state](self);
}

void SM_SetEffect(EffectSM_t *self, EffMacro_t *sequence, Color_t const *startColor, const uint8_t *intens,
                  const uint8_t delayedStart) {
    if (startColor != NO_COLOR) {
        SetColor(&self->curentColor, *startColor);
    } else {
        SetColor(&self->curentColor, *sequence->color);
    }
    SetColor(&self->lastColor, self->curentColor);
    if (intens != (uint8_t *)NULL) {
        self->SMIParams.idleIntens = *intens;
    }

    self->SMPValues.macroIdx = 0;

    self->p_effSeq = sequence;
    if (delayedStart > 0) {
        self->p_effMac       = delayPrequel;
        self->SMPValues.tick = delayedStart;
    } else {
        self->p_effMac       = sequence;
        self->SMPValues.tick = self->p_effMac->duration;
    }

    self->SMPValues.tick++;
    self->SMPValues.dissolveCnt = 0;
    self->SMPValues.repeats     = self->p_effMac->repeats;
    SetIndexes(self);
}
