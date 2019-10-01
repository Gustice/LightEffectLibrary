
#pragma once

#include "Color.h"
#include <stdint.h>

enum e_EffectPart {
    Light_Idle = 0,
    Light_Wave,
    Light_InvWave,
    Light_Flicker,
    Light_Sparkle,
    Light_Freeze,
    Light_Blank,

    Light_States,
};

typedef const struct EffectMacroElement_def {
    e_EffectPart   state;          // SubEffect
    uint8_t const *pu8_wave;       // waveform reference
    uint8_t        u8_FSintensity; // Full-Scale-intensity
    uint8_t        u8_duration;    // Effect duration
    const Color_t *color;          // SubEffect color
    int8_t         u8_repeats;     // Num of Repeats
    int8_t         next;           // Next SubEffect
} EffMacro_type;

class EffMacro {
  public:
    EffMacro(e_EffectPart state, uint8_t const *pu8_wave, uint8_t u8_FSintensity, uint8_t u8_duration,
             Color const *color, int8_t u8_repeats, int8_t next);

    e_EffectPart   _state;          // SubEffect
    uint8_t const *_pu8_wave;       // waveform reference
    uint8_t        _u8_FSintensity; // Full-Scale-intensity
    uint8_t        _u8_duration;    // Effect duration
    Color const *  _color;          // SubEffect color
    int8_t         _u8_repeats;     // Num of Repeats
    int8_t         _next;           // Next SubEffect
};
class DualEffMacro : EffMacro {
  public:
    DualEffMacro(e_EffectPart state, e_EffectPart stateI, uint8_t const *pu8_wave, uint8_t const *pu8_waveI,
                 uint8_t u8_FSintensity, uint8_t u8_duration, Color const *color, Color const *colorI,
                 int8_t u8_repeats,
                 int8_t next); // : EffMacro(state, pu8_wave, u8_FSintensity, u8_duration, color, u8_repeats, next);

    e_EffectPart   _stateI;    // SubEffect
    uint8_t const *_pu8_waveI; // waveform reference
    Color const *  _colorI;    // SubEffect color
};