
#include "EffectMacro.h"
#include "EffectWaveforms.h"

// EffMacro::EffMacro(e_EffectPart state,
//     uint8_t const * pu8_wave,
//     uint8_t u8_FSintensity,
//     uint8_t u8_duration,
//     Color const * color,
//     int8_t u8_repeats,
//     int8_t next
//     ){
//         _state = state;
//         _pu8_wave = pu8_wave;
//         _u8_FSintensity = u8_FSintensity;
//         _u8_duration = u8_duration;
//         _color = color;
//         _u8_repeats = u8_repeats;
//         _next = next;
//     }

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
    {Light_Idle, (uint8_t *)0, 0xFF, 32, USEOLD_COLOR, 0, 1},
};

EffMacro_t eff_Idle[] = {
    {Light_Idle, (uint8_t *)0, 0xFF, 32, &color_ColdWhite, 0, 0},
};

EffMacro_t eff_StdPulse[] = {
    {Light_Idle, (uint8_t *)0, 0, 32, &color_ColdWhite, 0, 1},
    {Light_Wave, gau8_offsetPulse, 0xFF, 32, USEOLD_COLOR, 0, 2},
    {Light_Idle, (uint8_t *)0, 0, 32, USEOLD_COLOR, 0, 0},
};

EffMacro_t eff_NervousPulse[] = {
    {Light_Idle, (uint8_t *)0, 0, 4, &color_ColdWhite, 0, 1},
    {Light_Wave, gau8_offsetPulse, 0xFF, 8, USEOLD_COLOR, 0, 2},
    {Light_Idle, (uint8_t *)0, 0, 4, USEOLD_COLOR, 0, 0},
};

EffMacro_t eff_StdWipe[] = {
    {Light_Idle, (uint8_t *)0, 0, 32, &color_ColdWhite, 0, 1},

    {Light_Wave, gau8_offsetSlope, 0xFF, 32, USEOLD_COLOR, 0, 2},
    {Light_Freeze, (uint8_t *)0, 0, 32, USEOLD_COLOR, 0, 3},
    {Light_RevWave, gau8_offsetSlope, 0xFF, 32, USEOLD_COLOR, 0, 4},
    {Light_Freeze, (uint8_t *)0, 0, 32, USEOLD_COLOR, 0, 1},
};

// EffMacro efm_StdPulse[] = {
//     EffMacro(Light_Idle, (uint8_t *)0, 0, 32, &_ctColdWhite, 0, 1),
//     EffMacro(Light_Wave, gau8_offsetPulse, 0xFF, 32, USEOLD_COLOR_OBJ, 0, 2),
//     EffMacro(Light_Idle, (uint8_t *)0, 0, 32, USEOLD_COLOR_OBJ, 0, 0),
// };

const static Effect1 someEffects[] = {
    Effect1(Light_Blank, 1, 2),
    Effect2(Light_Blank, 1, 2, gau8_offsetPulse, 0, 0),
    Effect1(Light_Blank, 1, 2),
};
const static Effect3 someInit(Light_Blank, 1, 2, nullptr, 0, 0, 0, 0, NO_COLOR, 128, 10);
EffectMacro          someScene(&someInit, someEffects, 3);

const static Effect1 startEffect[] = {
    Effect1(Light_Blank, 64, 0),
};
EffectMacro idleScene(nullptr, startEffect, 1);

} // namespace Effect
