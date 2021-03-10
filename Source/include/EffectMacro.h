
#pragma once

#include "Color.h"
#include "EffectBasics.h"
#include "EffectStateMachine.h"
#include <stdint.h>

namespace Effect {

extern EffMacro_t eff_Dark[];
extern EffMacro_t eff_StartIdle[];
extern EffMacro_t eff_Idle[];
extern EffMacro_t eff_StdPulse[];
extern EffMacro_t eff_NervousPulse[];
extern EffMacro_t eff_StdWipe[];
extern EffMacro_t eff_AsymPulse[];
extern EffMacro_t eff_ShiftAsymPulse[];

extern EffectMacro someScene;
extern EffectMacro idleScene;

extern const EffectMacro StdPulse[];
extern const EffectMacro StdDark[];


} // namespace Effect