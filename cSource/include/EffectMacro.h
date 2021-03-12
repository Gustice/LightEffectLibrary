#pragma once
#ifdef __cplusplus 
extern "C"{
#endif

#include "color_t.h"
#include "EffectBasics.h"
#include "EffectStateMachine.h"
#include <stdint.h>

extern EffMacro_t eff_Dark[];
extern EffMacro_t eff_StartIdle[];
extern EffMacro_t eff_Idle[];
extern EffMacro_t eff_StdPulse[];
extern EffMacro_t eff_NervousPulse[];
extern EffMacro_t eff_StdWipe[];
extern EffMacro_t eff_AsymPulse[];
extern EffMacro_t eff_ShiftAsymPulse[];

#ifdef __cplusplus
}
#endif