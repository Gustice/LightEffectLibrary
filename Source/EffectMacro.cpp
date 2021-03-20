
#include "EffectMacro.h"
#include "EffectWaveforms.h"

/************************************************************************/
/* Predefined effects                                                   */
/************************************************************************/

namespace Effect {

// EffMacro_type
// state            wave				FS		duration    color	            repeats     next
EffMacro_t eff_Dark[] = {
    {Light_Idle, (uint8_t *)0, 0, 32, &color_Black, 0, 0},
};

EffMacro_t eff_StartIdle[] = {
    {Light_Wave, gau8_initSlope, 0xFF, 16, &color_ColdWhite, 0, 1},
    {Light_Idle, (uint8_t *)0, 0xFF, 32, oldColor, 0, 1},
};

EffMacro_t eff_Idle[] = {
    {Light_Idle, (uint8_t *)0, 0xFF, 32, &color_ColdWhite, 0, 0},
};

EffMacro_t eff_StdPulse[] = {
    {Light_Idle, (uint8_t *)0, 0, 32, &color_ColdWhite, 0, 1},
    {Light_Wave, gau8_offsetPulse, 0xFF, 32, oldColor, 0, 2},
    {Light_Idle, (uint8_t *)0, 0, 32, oldColor, 0, 0},
};

EffMacro_t eff_NervousPulse[] = {
    {Light_Idle, (uint8_t *)0, 0, 4, &color_ColdWhite, 0, 1},
    {Light_Wave, gau8_offsetPulse, 0xFF, 8, oldColor, 0, 2},
    {Light_Idle, (uint8_t *)0, 0, 4, oldColor, 0, 0},
};

EffMacro_t eff_StdWipe[] = {
    {Light_Idle, (uint8_t *)0, 0, 32, &color_ColdWhite, 0, 1},

    {Light_Wave, gau8_offsetSlope, 0xFF, 32, oldColor, 0, 2},
    {Light_Freeze, (uint8_t *)0, 0, 32, oldColor, 0, 3},
    {Light_RevWave, gau8_offsetSlope, 0xFF, 32, oldColor, 0, 4},
    {Light_Freeze, (uint8_t *)0, 0, 32, oldColor, 0, 1},
};

const EffectMacro StdPulse[] = {
    EffectMacro(32, 1, Light_Idle, &color_White),
    EffectMacro(32, 2, gau8_offsetPulse, eEffect::Light_Wave),
    EffectMacro(32, 1) // <- Back to one to not change color
};

const EffectMacro StdDark[] = {
    EffectMacro(32, 1, Light_Idle, &color_Black),
};

} // namespace Effect
