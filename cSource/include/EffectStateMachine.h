#pragma once
#ifdef __cplusplus 
extern "C"{
#endif

#include "color_t.h"
#include "EffectBasics.h"
#include <stdint.h>

typedef struct {
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

typedef struct {
    /// Idle intensity as background color
    uint8_t idleIntens;
    /// dynamic range as variation around background color
    uint8_t dynamicRange;
    /// Fade steps for color changes
    uint8_t fadeSteps;
    /// Waveform template length -> index steps are calculated accordingly by give duration
    uint16_t templateLength;
} SM_ParameterValues_t;

/**
 * @brief Effect State Machine Class
 * @details Processes Effect macro array Macro entry subsequently
 * Processing is executed in following manner:
 *  \li Index-incrementing each Tick
 *      \li ticks until duration \ref EffMacro is finished
 *      \li each tick the index incremented by templateLength/duration
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
     * @param templateLength Length of waveforms that are used to display
     */
    EffectSM(uint16_t const templateLength) : EffectSM(templateLength, 0, 0){};
    ~EffectSM();

    EffectSM(uint16_t const templateLength, uint8_t const intensity, uint8_t const crossFade);
    /// @todo Configuration could be given by structure

    void SetEffect(EffMacro_t *sequence, Color_t const *startColor = noColor, uint8_t initialDelay = 0);
    void SetEffect(EffMacro_t *sequence, Color_t const *startColor, const uint8_t *intens, const uint8_t delayedStart);
    void SetEffect(const EffectMacro *sequence);

    virtual Color const *Tick(void);
    uint8_t              GetDissolveRatio(void);

    /**
     * @brief Set the Dynamic Range of effects
     * @param range Dynamic range 0-255. Which applied around idle brightness
     */
    /// @todo Fill this with some magic
    void SetDynamicRange(uint8_t range) { SMIParams.dynamicRange = range; };

    /**
     * @brief Get the index for the current waveform
     * @details The index is calculated with a higher accuracy in the background.
     * @return const uint8_t index to waveform position.
     */
    const uint8_t GetWaveIdx(void) { return ((SMPValues.waveIdx & 0xFF00u) >> 8); };

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
    const uint8_t GetIntensity(void) { return SMIParams.idleIntens; };

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

    /// Last color
    Color _lastColor;

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


#ifdef __cplusplus
}
#endif