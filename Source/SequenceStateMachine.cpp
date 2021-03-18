/**
 * @file SequenceStateMachine.cpp
 * @author Gustice
 * @brief Implementation of Effect-State-Machine-Class SequenceStateMachine.h
 * @version 0.1
 * @date 2021-03-12
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "SequenceStateMachine.h"
#include "EffectWaveforms.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

namespace Effect {

const EffectMacro delaySequence(1, 0);

SequenceSM::SequenceSM(uint16_t const templateLength, uint8_t targetCount, uint8_t const intensity, uint8_t crossFade)
    : _templateLength(templateLength), _targetCount(targetCount) {
    assert(targetCount > 0);
    SMIParams.templateLength = templateLength;
    SMIParams.idleIntens     = intensity;
    SMIParams.fadeSteps      = 0;
    SMIParams.dynamicRange   = 30;
    if (crossFade > 0)
        SMIParams.fadeSteps = crossFade; // @todo
    _outputColor = new Color[_targetCount];
}

/**
 * @brief Destroy the Effect State Machine
 *
 */
SequenceSM::~SequenceSM() { delete[] _outputColor; }

void SequenceSM::SetEffect(const EffectMacro *sequence, Color_t const *startColor, uint8_t initialDelay) {
    SetEffect(sequence, startColor, &SMIParams.idleIntens, initialDelay);
}

/**
 * @brief Sets the effect state machine to process given effect macro
 *
 * @param sequence
 * @param startColor
 * @param delayedStart
 * @param intens
 */
void SequenceSM::SetEffect(const EffectMacro *sequence, Color_t const *startColor, const uint8_t *intens,
                           const uint8_t delayedStart) {
    if (startColor != NO_COLOR) {
        _curentColor.SetColor(*startColor);
    } else {
        _curentColor.SetColor(*sequence->pColor);
    }
    _lastColor.SetColor(_curentColor.GetColor());
    if (intens != nullptr) {
        SMIParams.idleIntens = *intens;
    }

    SMPValues.macroIdx = 0;

    _p_effSeq = sequence;
    if (delayedStart > 0) {
        _p_effMac      = &delaySequence;
        SMPValues.tick = delayedStart;
    } else {
        _p_effMac      = sequence;
        SMPValues.tick = _p_effMac->duration;
    }

    SMPValues.tick++;
    SMPValues.dissolveCnt = 0;
    SMPValues.repeats     = _p_effMac->repeats;
    this->SetIndexes();
}

/**
 * @brief Executes one tick of the statemachine
 * @details Each tick the tick variable is decremented. After the limit is reached,
 *  the current macro line is either repeated or the next macro line is started (with
 *  an optional color change).\n
 * If the color is changed by switching the macro line, a dissolve counterr is concurrently
 *  triggered (see \ref GetDissolveRatio). This counter can be used to cross fade between
 *  different colors.
 *
 * @note Tick must be called regularly. For standard light applications all 40 ms seems to be
 *  a convenient values.
 *
 * @return uint8_t
 */
Color *SequenceSM::Tick(void) {
    // tick-increment
    if (--SMPValues.tick == 0) {
        // repeats-decrement
        if (SMPValues.repeats-- == 0) {
            SMPValues.macroIdx = _p_effMac->next;
            _p_effMac          = &(_p_effSeq[_p_effMac->next]);
            SMPValues.repeats  = _p_effMac->repeats;

            // execute color change if necessary
            if (_p_effMac->pColor != NO_COLOR) {
                SMPValues.dissolveCnt = SMIParams.fadeSteps;
                _lastColor.SetColor(_curentColor.GetColor());
                _curentColor.SetColor(*_p_effMac->pColor);
            }
        }

        SMPValues.tick = _p_effMac->duration;
        this->SetIndexes();
    }

    SMPValues.waveIdx += SMPValues.waveStep;
    if (SMPValues.dissolveCnt > 0) {
        SMPValues.dissolveCnt--;
    }

    switch (_p_effMac->state) {
    case eEffect::Light_Blank:
        UpdateBlank();
        break;
    case eEffect::Light_Idle:
        UpdateIdle();
        break;
    case eEffect::Light_Freeze:
        UpdateFreeze();
        break;
    case eEffect::Light_Wave:
        UpdateWave();
        break;
    case eEffect::Light_RevWave:
        UpdateRevWave();
        break;
    case eEffect::Light_Flicker:
        UpdateFlicker();
        break;
    case eEffect::LightCustom: {
        EffectMacro const *const cEffStep = GetStep();
        return cEffStep->pProcessor(this, _outputColor, _targetCount);
    } break;
    default:

        break;
    }

    return _outputColor;
}

/**
 * @brief Sets Waveform index according du desired duration, waveform-length and current step
 */
void SequenceSM::SetIndexes(void) {
    SMPValues.waveStep = ((SMIParams.templateLength) << 8);
    SMPValues.waveStep /= _p_effMac->duration;
    SMPValues.waveIdx = (uint16_t)(0 - SMPValues.waveStep);
}

/**
 * @brief Returns fading ramp in cases the color were switched
 * @return uint8_t
 */
/// @todo it seems not te be very clever to delegate this on higher level
uint8_t SequenceSM::GetDissolveRatio(void) {
    if (SMIParams.fadeSteps == 0)
        return 0;

    uint8_t dissolving = (uint8_t)((uint16_t)0xFF * SMPValues.dissolveCnt / SMIParams.fadeSteps);
    return dissolving;
}

void SequenceSM::UpdateBlank() {
    Color cOut = _curentColor * 0;
    ApplyColorToAllElements(cOut);
}

void SequenceSM::UpdateIdle() {
    Color cOut = _curentColor * SMIParams.idleIntens;
    ApplyColorToAllElements(cOut);
}

void SequenceSM::UpdateFreeze() {
    // just do nothing and Return currently set color
}

void SequenceSM::UpdateWave() {
    EffectMacro const *const cEffStep = GetStep();
    Color                    cOut     = GetColor() * cEffStep->pWave[GetWaveIdx()] * cEffStep->FsIntensity;

    ApplyColorToAllElements(cOut);
}

void SequenceSM::UpdateRevWave() {
    EffectMacro const *const cEffStep  = GetStep();
    uint8_t                  lastIndex = SMIParams.templateLength - 1;
    Color                    cOut      = GetColor() * cEffStep->pWave[lastIndex - GetWaveIdx()] * cEffStep->FsIntensity;

    ApplyColorToAllElements(cOut);
}

void SequenceSM::UpdateFlicker() {
    uint8_t k;
    k          = rand();
    k          = SMIParams.idleIntens + ((int16_t)k * (SMIParams.dynamicRange) / 0xFF);
    Color cOut = _curentColor * k;
    ApplyColorToAllElements(cOut);
}

void SequenceSM::ApplyColorToAllElements(Color &color) {
    for (size_t i = 0; i < _targetCount; i++) {
        _outputColor[i] = color;
    }
}

Color const *LightSparkleSequence(SequenceSM *obj, Color *colors, size_t len) {
    const Color cColor = obj->GetColor();
    uint8_t     k, kr;

    kr      = ((int16_t)rand() * 14 / 0xFF);
    k       = 50 + kr * kr;
    *colors = cColor * k;
    return colors;
}

Color const *LightSwipeSequence(SequenceSM *obj, Color *colors, size_t len) {
    EffectMacro const *const cEffStep = obj->GetStep();
    const Color              cColor   = obj->GetColor();
    uint8_t                  cStep    = obj->GetWaveIdx();
    uint8_t                  step     = obj->_templateLength;

    colors[0] = cColor * cEffStep->pWave[cStep] * cEffStep->FsIntensity;

    if (len < 2)
        return colors;
    colors[1] = cColor * cEffStep->pWave[obj->_templateLength - 1 - step] * cEffStep->FsIntensity;

    return colors;
}

Color const *LightWaveSequence(SequenceSM *obj, Color *colors, size_t len) {
    EffectMacro const *const cEffStep = obj->GetStep();
    const Color              cColor   = obj->GetColor();
    uint8_t                  cStep    = obj->GetWaveIdx();
    uint8_t                  step     = obj->_templateLength / len;

    for (int i = 0; i < len; i++) {
        colors[i] = cColor * cEffStep->pWave[cStep] * cEffStep->FsIntensity;
        cStep += step;
    }

    return colors;
}

} // namespace Effect
