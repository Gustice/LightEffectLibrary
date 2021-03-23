/**
 * @file EffectMacro.h
 * @author Gustice
 * @brief Predefined effect macros 
 * @version 0.1
 * @date 2021-03-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#ifdef __cplusplus 
extern "C"{
#endif

#include "color_t.h"
#include "EffectBasics.h"
#include "EffectStateMachine.h"
#include <stdint.h>

/// Dark - Nothing to see here
extern EffMacro_t eff_Dark[];
/// Smooth turn on and then remaining idle
extern EffMacro_t eff_StartIdle[];
/// Continuous idle 
extern EffMacro_t eff_Idle[];
/// Slow pulse starting from idle intensity. Appears like breathing 
extern EffMacro_t eff_StdPulse[];
/// Short pulses starting from idle intensity. Appears nervousely.
extern EffMacro_t eff_NervousPulse[];

#ifdef __cplusplus
}
#endif