/**
 * @file EffectMacro.c
 * @author Gustice
 * @brief Predefined Macros
 * @version 0.6
 * @date 2021-03-23
 * 
 * @copyright Copyright (c) 2021
 */

#include "EffectMacro.h"
#include "EffectWaveforms.h"

// EffMacro_type
// state            wave				FS		duration    color	            repeats     next
EffMacro_t eff_Dark[] = {
    {Light_Blank, (uint8_t *)0, 0, 32, &cBlack, 0, 0},
};

EffMacro_t eff_StartIdle[] = {
    {Light_Wave, gau8_initSlope, 0xFF, 16, &cColdWhite, 0, 1},
    {Light_Idle, (uint8_t *)0, 0xFF, 32, USEOLD_COLOR, 0, 1},
};

EffMacro_t eff_Idle[] = {
    {Light_Idle, (uint8_t *)0, 0xFF, 32, NO_COLOR, 0, 0},
};

EffMacro_t eff_StdPulse[] = {
    {Light_Idle, (uint8_t *)0, 0, 32, NO_COLOR, 0, 1},
    {Light_Wave, gau8_offsetPulse, 0xFF, 32, USEOLD_COLOR, 0, 2},
    {Light_Idle, (uint8_t *)0, 0, 32, USEOLD_COLOR, 0, 0},
};

EffMacro_t eff_NervousPulse[] = {
    {Light_Idle, (uint8_t *)0, 0, 4, NO_COLOR, 0, 1},
    {Light_Wave, gau8_offsetPulse, 0xFF, 8, USEOLD_COLOR, 0, 2},
    {Light_Idle, (uint8_t *)0, 0, 4, USEOLD_COLOR, 0, 0},
};
