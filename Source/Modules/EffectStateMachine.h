/**
 * @file EffectStateMachine.h
 * @author Gustice
 * @brief Effect-State-Machine-Class for processing Effect Macros
 * @version 0.1
 * @date 2019-10-01
 *
 * @copyright Copyright (c) 2019
 *
 */
#pragma once

#include "Color.h"
#include "EffectBasics.h"
#include <stdint.h>

namespace Effect {

typedef struct SM_ProcessValues_def {
    /// Decrementing tick, counting duration
    uint8_t u8_tick;
    /// Decrementing repeats of current Effect-part
    uint8_t u8_repeats;
    /// Current index to waveform
    uint8_t u16_waveIdx;
    /// Index step each tick according to duration and template length.
    uint8_t u16_waveStep;
    /// Dissolve counter dissolves the leading color each tick
    uint8_t u8_dissolveCnt;
    /// Index to macro element
    uint8_t u8_macroIdx;
} SM_ProcessValues_t;

typedef struct SM_ParamValues_def {
    /// Idle intensity as background color
    uint8_t u8_idleIntens;
    /// dynamic range as variation around background color
    uint8_t u8_dynamicRange;
    /// Fade steps for color changes
    uint8_t u8_fadeSteps;
    /// Waveform template length -> index steps are calculated accordingly by give duration
    uint16_t u16_templateLength;
} SM_ParameterValues_t;

/**
 * @brief Effect State Machine Class
 * @details Processes Effect macro array Macro entry subsequently
 * Processing is executed in following manner:
 *  \li Index-incrementing each Tick
 *      \li ticks until duration \ref EffMacro is finished
 *      \li each tick the index incremented by templateLenght/duration
 *  \li Repeats-processing
 *      \li Each repeat the tick and the index are set back to start
 *      \li Switches to next Macro enty
 *      \li Next entry is given by 'next' index
 *      \li Color changes can be executed optionally
 */
class EffectSM {
  public:
    /**
     * @brief Construct a new Effect State Machine object
     * @param templateLength
     */
    EffectSM(uint16_t const templateLength) : EffectSM(templateLength, 0, 0){};

    EffectSM(uint16_t const templateLength, uint8_t const intensity, uint8_t const crossFade);
    /// @todo Configuration could be given by structure

    void SetEffect(EffMacro_t *sequence, Color_t const *startColor = NO_COLOR, uint8_t intialDelay = 0);
    void SetEffect(EffMacro_t *sequence, Color_t const *startColor, uint8_t delayedStart, uint8_t intens);
    void SetEffect(const EffectMacro *sequence);

    virtual Color const *Tick(void);
    uint8_t      GetDissolveRatio(void);

    /**
     * @brief Set the Dynamic Range of effects
     * @param range Dynamic range 0-255. Which applied around idle brightness
     */
    /// @todo Fill this with some magic
    void SetDynamicRange(uint8_t range) { SMIParams.u8_dynamicRange = range; };

    /**
     * @brief Get the index for the current waveform
     * @details The index is calculated with a higher accuracy in the background.
     * @return const uint8_t index to waveform position.
     */
    const uint8_t GetWaveformIdx(void) { return ((SMPValues.u16_waveIdx & 0xFF00u) >> 8); };

    /**
     * @brief Get current color
     * @return const Color
     */
    /// @todo delete?
    const Color GetColor(void) { return _curentColor; };

    /**
     * @brief Get set intensity
     * @return const uint8_t intensity
     */
    /// @todo delete?
    const uint8_t GetIntensity(void) { return SMIParams.u8_idleIntens; };

    /**
     * @brief Get index to current macro in sequence
     * @return EffMacro_type const* const
     */
    EffMacro_t const *const GetStep(void) { return _p_effMac; };

    /**
     * @brief Get Process values of state machen
     *
     * @return const SM_ProcessValues_t
     */
    const SM_ProcessValues_t GetProcessValues(void) { return SMPValues; };

    /// @todo change to protected and tests results
  protected:
    /// First element of Effect macro stack
    EffMacro_t const *_p_effSeq;
    /// Currently indexed Effect-part
    EffMacro_t const *_p_effMac;

    Color *_outputColor; /// @todo !!

    // Color& (*apF_Effects)(EffectSM * SM);
    typedef Color const * pEffPrc(EffectSM *);
    static pEffPrc *const apF_Processors[6];
    static Color const *  UpdateBlank(EffectSM *SM);
    static Color const *  UpdateIdle(EffectSM *SM);
    static Color const *  UpdateFreeze(EffectSM *SM);
    static Color const *  UpdateWave(EffectSM *SM);
    static Color const *  UpdateRevWave(EffectSM *SM);
    static Color const *  UpdateFlicker(EffectSM *SM);

    /// Current color
    /// \li Either forced by start with parameter
    /// \li Or given by current Effect-part
    Color _curentColor;

    /// Concentrated parameter values of instance
    SM_ParameterValues_t SMIParams;

    /// Concentrated process values of instance
    SM_ProcessValues_t SMPValues;

    /// @todo callback for finished to exclude these indexes

    void SetIndexes(void);

    EffectSM(void){};
};


class SequenceSM : public EffectSM
{
public:
  SequenceSM(const Effect1 * init);
  void SetEffect(const EffectMacro *sequence, uint8_t delay);

  Color const *Tick(void);

protected:
    /// First element of Effect macro stack
    EffectMacro const *p_effSeq;
    /// Currently indexed Effect-part
    Effect1 const *p_effMac;

    void SetupProcessVariables(Effect1 const *p_effMac);

    typedef Color const * pSeqPrc(SequenceSM *);
    static pSeqPrc *const apF_SqProcessors[6];
    static Color const *  UpdateBlank(SequenceSM *SM);
    static Color const *  UpdateIdle(SequenceSM *SM);
    static Color const *  UpdateFreeze(SequenceSM *SM);
    static Color const *  UpdateWave(SequenceSM *SM);
    static Color const *  UpdateRevWave(SequenceSM *SM);
    static Color const *  UpdateFlicker(SequenceSM *SM);



};


} // namespace Effect