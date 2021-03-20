/**
 * @file SequenceStateMachine.h
 * @author Gustice
 * @brief Effect-State-Machine-Class for processing Effect Macros
 * @version 0.1
 * @date 2021-03-12
 *
 * @copyright Copyright (c) 2021
 *
 */
#pragma once

#include "Color.h"
#include "EffectBasics.h"
#include <stdint.h>

namespace Effect {

class SequenceSM {
  public:
    /**
     * @brief Construct a new Effect State Machine object
     * @param templateLength Length of waveforms that are used to display
     */
    SequenceSM(uint16_t const templateLength, uint8_t targetCount) : SequenceSM(templateLength, targetCount, 0, 0){};
    ~SequenceSM();

    SequenceSM(uint16_t const templateLength, uint8_t targetCount, uint8_t const intensity, uint8_t const crossFade);
    /// @todo Configuration could be given by structure

    void SetEffect(const EffectMacro *sequence, Color_t const *startColor = NO_COLOR, uint8_t initialDelay = 0);
    void SetEffect(const EffectMacro *sequence, Color_t const *startColor, const uint8_t *intens,
                   const uint8_t delayedStart);

    virtual Color *Tick(void);
    uint8_t        GetDissolveRatio(void);

    const size_t _templateLength;
    const size_t _targetCount;

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
    EffectMacro const *const GetStep(void) { return _p_effMac; };

    /**
     * @brief Get Process values of state machen
     *
     * @return const SM_ProcessValues_t
     */
    const SM_ProcessValues_t GetProcessValues(void) { return SMPValues; };

    /// @todo change to protected and tests results
  protected:
    /// First element of Effect macro stack
    EffectMacro const *_p_effSeq;
    /// Currently indexed Effect-part
    EffectMacro const *_p_effMac;

    Color *_outputColor; /// @todo !!

    // Color& (*apF_Effects)(SequenceSM * SM);
    void UpdateBlank();
    void UpdateIdle();
    void UpdateFreeze();
    void UpdateWave();
    void UpdateRevWave();
    void UpdateFlicker();

    void ApplyColorToAllElements(Color &color);

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
};

Color *LightSparkleSequence(SequenceSM *obj, Color *colors, size_t len);
Color *LightSwipeSequence(SequenceSM *obj, Color *colors, size_t len);
Color *LightWaveSequence(SequenceSM *obj, Color *colors, size_t len);

} // namespace Effect