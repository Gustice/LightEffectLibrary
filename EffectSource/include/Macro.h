
#pragma once

#include "Color.h"
#include "Basics.h"
#include "StateMachine.h"
#include <stdint.h>

namespace Effect {

extern Macro_t macDark[];
extern Macro_t macStartIdle[];
extern Macro_t macIdle[];
extern Macro_t macStdPulse[];
extern Macro_t macNervousPulse[];
extern Macro_t macStdWipe[];
extern Macro_t macAsymPulse[];
extern Macro_t macShiftAsymPulse[];

extern Sequence someScene;
extern Sequence idleScene;

extern const Sequence StdPulse[];
extern const Sequence StdDark[];


} // namespace Effect