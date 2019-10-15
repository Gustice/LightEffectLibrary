#pragma once

#include <stdint.h>
#include "Color.h"

namespace Effect {

enum e_EffectPart {
    Light_Blank = 0,
    Light_Idle,
    Light_Freeze,
    Light_Wave,
    Light_RevWave,
    Light_Flicker,

    Light_States,
};

typedef const struct EffectMacroElement_def {
    e_EffectPart state;          // SubEffect
    uint8_t const *        pu8_wave;       // waveform reference
    uint8_t                u8_FSintensity; // Full-Scale-intensity
    uint8_t                u8_duration;    // Effect duration
    const Color_t *        color;          // SubEffect color
    int8_t                 u8_repeats;     // Num of Repeats
    int8_t                 next;           // Next SubEffect
} EffMacro_type;

}