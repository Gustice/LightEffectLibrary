#pragma once

#include "Color.h"
#include <stdint.h>

namespace Effect {

enum eEffect {
    Light_Blank = 0,
    Light_Idle,
    Light_Freeze,
    Light_Wave,
    Light_RevWave,
    Light_Flicker,

    Light_States,
};

enum eMacroLevel {
    level0,
    level1,
    level2,
    level3,
};

typedef const struct Macro1_def {
    eMacroLevel level;
    eEffect     state;          // SubEffect
    uint8_t     u8_duration;    // Effect duration
    int8_t      next;           // Next SubEffect
    uint8_t     u8_FSintensity; // Full-Scale-intensity

} EffMacro1_t;

typedef const struct Macro2_def {
    EffMacro1_t state; // SubEffect

    uint8_t const *pu8_wave;   // waveform reference
    const Color_t *color;      // SubEffect color
    int8_t         u8_repeats; // Num of Repeats
} EffMacro2_t;

typedef const struct EffectMacroElement_def {
    eEffect        state;          // SubEffect
    uint8_t const *pu8_wave;       // waveform reference
    uint8_t        u8_FSintensity; // Full-Scale-intensity

    uint8_t        u8_duration; // Effect duration
    const Color_t *color;       // SubEffect color
    int8_t         u8_repeats;  // Num of Repeats
    int8_t         next;        // Next SubEffect
} EffMacro_t;
#define COUNT_EFFECT_ELEMENT(effect) (sizeof(effect) / sizeof(EffMacro_type))

class EffectBase {
  public:
    ~EffectBase() = default;

  protected:
    EffectBase(eMacroLevel lev) : level(lev) {}

  private:
    const eMacroLevel level;
};

typedef struct EffLevel1_def
{
    const eEffect effect;
    const uint8_t duration;
    const uint8_t next;
}EffLevel1_t;






class Effect1 : public EffectBase {
  public:
    Effect1(eEffect eff, uint8_t dur, uint8_t nxt) : EffectBase(eMacroLevel::level1), state(eff), duration(dur), next(nxt) {}
    virtual ~Effect1() = default;

    const eEffect state;
    const uint8_t duration;
    const uint8_t next;

  protected:
    Effect1(eMacroLevel lev, eEffect eff, uint8_t dur, uint8_t nxt) : EffectBase(lev), state(eff), duration(dur), next(nxt) {}
};

class Effect2 : public Effect1 {
  public:
    Effect2(eEffect eff, uint8_t dur, uint8_t nxt, const uint8_t *const wave, uint8_t dyn, uint8_t rep)
        : Effect1(eMacroLevel::level2, eff, dur, nxt), p_wave(wave), dynRange(dyn), repeats(rep) {}

    virtual ~Effect2() = default;

    const uint8_t *const p_wave;
    const uint8_t        dynRange;
    const uint8_t        repeats;

  protected:
    Effect2(eMacroLevel lev, eEffect eff, uint8_t dur, uint8_t nxt, const uint8_t *const wave, uint8_t dyn, uint8_t rep)
        : Effect1(lev, eff, dur, nxt), p_wave(wave), dynRange(dyn), repeats(rep) {}

};

class Effect3 : public Effect2 {
  public:
    Effect3(eEffect eff, uint8_t dur, uint8_t nxt, uint8_t const *wave, uint8_t dyn, uint8_t rep, uint8_t intens, uint8_t idle, Color_t *newCol, uint8_t len,
            uint8_t fade)
        : Effect2(eMacroLevel::level3, eff, dur, nxt, wave, dyn, rep), fsIntensity(intens), idleIntens(idle), color(newCol), templateLen(len), fadeSteps(fade) {
    }
    virtual ~Effect3() = default;

    const uint8_t  fsIntensity;
    const uint8_t  idleIntens;
    const Color_t *color;
    const uint8_t  templateLen;
    const uint8_t  fadeSteps;

  protected:
    Effect3(eMacroLevel lev, eEffect eff, uint8_t dur, uint8_t nxt, uint8_t const *wave, uint8_t dyn, uint8_t rep, uint8_t intens, uint8_t idle,
            Color_t *newCol, uint8_t len, uint8_t fade)
        : Effect2(lev, eff, dur, nxt, wave, dyn, rep), fsIntensity(intens), idleIntens(idle), color(newCol), templateLen(len), fadeSteps(fade) {}
};

class EffectMacro {
  public:
    EffectMacro(const Effect1 *init, const Effect1 *seq, uint8_t len) : initEffect(init), sequence(seq), sequLen(len) {}
    ~EffectMacro() = default;

    const Effect1 *initEffect;
    const Effect1 *sequence;
    const uint8_t  sequLen;
};



typedef struct EffectList_def {
    const EffMacro_t *Macro;    // Macro reference
    const uint8_t     u8_parts; // Macro-entries
} EffectList_type;

typedef struct EffektSequenceElement_def {
    const EffMacro_t *element;    // VisualisierungsElement
    uint8_t           u8_content; // Anzahl der eingetragenen Teilschritte
    uint8_t           u8_repeats; // Wiederholungen
} EffSequence_type;
#define COUNT_EFFECT_MAKRO(effect) (sizeof(effect) / sizeof(EffSequence_type))
extern const EffSequence_type em_Idle[];

} // namespace Effect