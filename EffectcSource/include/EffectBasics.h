#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "color_t.h"
#include <stdint.h>

typedef enum eEffect {
    Light_Blank = 0,
    Light_Idle,
    Light_Freeze,
    Light_Wave,
    Light_RevWave,
    Light_Flicker,

    Light_States,
} eEffect_t;

typedef const struct EffectMacroElement_def {
    /// Effect type
    eEffect_t state;
    /// waveform reference
    uint8_t const *pWave;
    /// Full-Scale-intensity
    uint8_t FsIntensity;
    /// Effect duration
    uint8_t duration;
    /// SubEffect color
    const Color_t *color;
    /// Num of Repeats
    int8_t repeats;
    /// Next SubEffect
    int8_t next;
} EffMacro_t;
#define COUNT_EFFECT_ELEMENT(effect) (sizeof(effect) / sizeof(EffMacro_type))

typedef struct EffectList_def {
    const EffMacro_t *Macro;    // Macro reference
    const uint8_t     u8_parts; // Macro-entries
} EffectList_type;

typedef struct EffectSequenceElement_def {
    const EffMacro_t *element;    // Element for visualization
    uint8_t           u8_content; // Number of registered steps
    uint8_t           u8_repeats; // Repetitions
} EffSequence_type;
#define COUNT_EFFECT_MACRO(effect) (sizeof(effect) / sizeof(EffSequence_type))
extern const EffSequence_type em_Idle[];

#ifdef __cplusplus
}
#endif