
#include "EffectMacro.h"

EffMacro::EffMacro(e_EffectPart state,
    uint8_t const * pu8_wave,
    uint8_t u8_FSintensity,
    uint8_t u8_duration,
    Color const * color,
    int8_t u8_repeats,
    int8_t next
    ){
        _state = state;
        _pu8_wave = pu8_wave;
        _u8_FSintensity = u8_FSintensity;
        _u8_duration = u8_duration;
        _color = color;
        _u8_repeats = u8_repeats;
        _next = next;
    }