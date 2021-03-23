/**
 * @file EffectProcessor.h
 * @author Gustice
 * @brief Definition of Effect-Processor
 * @version 0.1
 * @date 2021-03-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#ifdef __cplusplus 
extern "C"{
#endif

#include "Color_t.h"
#include "EffectStateMachine.h"
#include "EffectWaveforms.h"
#include <stdint.h>

/**
 * @brief Definition of effect processor. 
 * @details Manages colors and smooth cross-fading when effects are changed
 */
typedef struct EffectProcessor_def {
    /// Target count
    uint8_t colorSize;
    /// Current color
    Color_t   Color;
    /// Last applied color (used to crossfade in intermediate states)
    Color_t   ColorOld;
    /// Defines length of cross-fading 
    uint8_t fadeSteps;
    /// Current count of cross-fading effect
    uint8_t fadingCnt;
    /// Processing of current effect
    EffectSM_t EffPV;
    /// Processing of last effect (Last effect ist shortly kept alive to conduct cross fading)
    EffectSM_t EffPV_old;
}EffectProcessor_t;

/**
 * @brief Prepare EffectProcessor object
 * @param self Object reference
 * @param templateLength Length of waveform template
 * @param fadeSteps Steps count for cross fade
 */
void EP_Construct(EffectProcessor_t * self, uint16_t const templateLength, uint8_t const fadeSteps);

/**
 * @brief Set next effect sequence
 * @param self Object reference
 * @param sequence reference to sequence that has to be processed next
 * @param sColor Start color. Use NO_COLOR to start with default color
 * @param intens Idle intensity for effect. Use NULL to start with default intensity
 */
void EP_SetEffect(EffectProcessor_t * self, EffMacro_t *sequence, Color_t const *sColor, uint8_t const * intens);

/**
 * @brief Execute step of effect processor
 * @param self object reference
 * @return Reference to color result
 */
Color_t const * EP_Tick(EffectProcessor_t * self);

#ifdef __cplusplus
}
#endif