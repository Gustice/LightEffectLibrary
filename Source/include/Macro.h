
#pragma once

#include "Color.h"
#include "Basics.h"
#include "StateMachine.h"
#include <stdint.h>

namespace Effect {

extern Macro_t eff_Dark[];
extern Macro_t eff_StartIdle[];
extern Macro_t eff_Idle[];
extern Macro_t eff_StdPulse[];
extern Macro_t eff_NervousPulse[];
extern Macro_t eff_StdWipe[];
extern Macro_t eff_AsymPulse[];
extern Macro_t eff_ShiftAsymPulse[];

extern Sequence someScene;
extern Sequence idleScene;

extern const Sequence StdPulse[];
extern const Sequence StdDark[];


} // namespace Effect