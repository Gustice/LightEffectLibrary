/**
 * @file EffectBasics.h
 * @author Gustice
 * @brief Definitions for color effects
 * @version 0.6
 * @date 2021-03-21
 *
 * @copyright Copyright (c) 2021
 */

#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "color_t.h"
#include <stdint.h>

/**
 * @brief Effect that is applied to color
 */
typedef enum eEffect {
    /// Black out lights
    Light_Blank = 0,
    /// Continuous illumination in idle intensity
    Light_Idle,
    /// Keep latest state on hold
    Light_Freeze,
    /// Step through a defined waveform
    Light_Wave,
    /// Step through a defined waveform backwards
    Light_RevWave,
    /// Show flicker effect with idle intensity as static part with dynamic flicker-range
    Light_Flicker,

    Light_States,
} eEffect_t;

/**
 * @brief Definition of effect that is processed each step
 */
typedef const struct EffectMacroElement_def {
    /// Effect type
    eEffect_t state;
    /// waveform reference
    uint8_t const *pWave;
    /// Full-Scale-intensity
    uint8_t FsIntensity;
    /// Effect duration
    uint8_t duration;
    /// SubEffect color
    const Color_t *color;
    /// Num of Repeats
    int8_t repeats;
    /// Next SubEffect
    int8_t next;
} EffMacro_t;
#define COUNT_EFFECT_ELEMENT(effect) (sizeof(effect) / sizeof(EffMacro_type))

#ifdef __cplusplus
}
#endif