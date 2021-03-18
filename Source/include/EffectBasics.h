#pragma once

#include "Color.h"
#include <stdint.h>
#include "EffectWaveforms.h"

namespace Effect {

enum eEffect {
    Light_Blank = 0,
    Light_Idle,
    Light_Freeze,
    Light_Wave,
    Light_RevWave,
    Light_Flicker,
    LightCustom,
    Light_States,
};

typedef const struct EffMacro_def {
    /// Effect type
    eEffect state;
    /// waveform reference
    uint8_t const *pWave;
    /// Full-Scale-intensity
    uint8_t FsIntensity;
    /// Effect duration
    uint8_t duration;
    /// SubEffect color
    const Color_t *pColor;
    /// Num of Repeats
    int8_t repeats;
    /// Next SubEffect
    int8_t next;
} EffMacro_t;
#define COUNT_EFFECT_ELEMENT(effect) (sizeof(effect) / sizeof(EffMacro_type))


class SequenceSM;
/**
 * @brief Custom Sequence effect to process
 * @param obj Reference to object
 * @param len Target count of sequencer
 * @return typedef const* 
 */
typedef Color * (*pEffPrc)(SequenceSM * obj, Color * colors, size_t len);

class EffectMacro {
  public:
    /// Effect type
    const eEffect state;
    /// waveform reference
    const uint8_t * const pWave;
    /// Full-Scale-intensity
    const uint8_t FsIntensity;
    /// Effect duration
    const uint8_t duration;
    /// SubEffect color
    Color_t const *const pColor;
    /// Num of Repeats
    const int8_t repeats;
    /// Next SubEffect
    const int8_t next;
    /// Custom processor function
    const pEffPrc pProcessor;

    constexpr EffectMacro(uint8_t duration, uint8_t next, eEffect state = eEffect::Light_Freeze, Color_t const *pColor = USEOLD_COLOR)
        : state(state), pWave(NO_WAVE), FsIntensity(gu8_fullIntensity), duration(duration),
          pColor(pColor), repeats(0), next(next), pProcessor(nullptr){};

    constexpr EffectMacro(uint8_t duration, uint8_t next, uint8_t const *pWave, eEffect state, Color_t const *pColor = USEOLD_COLOR, uint8_t repeat = 0)
        : state(state), pWave(pWave), FsIntensity(gu8_fullIntensity), duration(duration), pColor(pColor),
          repeats(repeat), next(next), pProcessor(nullptr){};

    constexpr EffectMacro(uint8_t duration, uint8_t next, pEffPrc pProcessor, uint8_t const *pWave, Color_t const *pColor = USEOLD_COLOR, uint8_t repeat = 0)
        : state(eEffect::LightCustom), pWave(pWave), FsIntensity(gu8_fullIntensity), duration(duration), pColor(pColor),
          repeats(repeat), next(next), pProcessor(pProcessor) {};

    constexpr EffectMacro(EffMacro_t macro)
        : state(macro.state), pWave(macro.pWave), FsIntensity(macro.FsIntensity), duration(macro.duration),
          pColor(macro.pColor), repeats(macro.repeats), next(macro.next), pProcessor(nullptr){};
};

typedef struct {
    const EffMacro_t *element; // Element for visualization
    uint8_t           content; // Number of registered steps
    uint8_t           repeats; // Repetitions
} EffSequence_t;
#define COUNT_EFFECT_MACRO(effect) (sizeof(effect) / sizeof(EffSequence_type))
extern const EffSequence_t em_Idle[];


typedef struct SM_ProcessValues_def {
    /// Decrementing tick, counting duration
    uint8_t tick;
    /// Decrementing repeats of current Effect-part
    uint8_t repeats;
    /// Dissolve counter dissolves the leading color each tick
    uint8_t dissolveCnt;
    /// Index to macro element
    uint8_t macroIdx;
    /// Current index to waveform
    uint16_t waveIdx;
    /// Index step each tick according to duration and template length.
    uint16_t waveStep;
} SM_ProcessValues_t;

typedef struct SM_ParamValues_def {
    /// Idle intensity as background color
    uint8_t idleIntens;
    /// dynamic range as variation around background color
    uint8_t dynamicRange;
    /// Fade steps for color changes
    uint8_t fadeSteps;
    /// Waveform template length -> index steps are calculated accordingly by give duration
    uint16_t templateLength;
} SM_ParameterValues_t;


} // namespace Effect