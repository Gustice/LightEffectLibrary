/**
 * @file EffectStateMachine.cpp
 * @author Gustice
 * @brief Implementation of Effect-State-Machine-Class EffectStateMachine.h
 * @version 0.1
 * @date 2019-10-01
 *
 * @copyright Copyright (c) 2019
 *
 */
#include "EffectStateMachine.h"
#include "EffectWaveforms.h"
#include <math.h>
#include <stdlib.h>

namespace Effect {

/// Optional prequel for delayed start.
/// \li Duration must be != 0
/// \li Next entry will be first entry in given Effect sequence
EffMacro_t delayPrequel[] = {
    {Light_Blank, (uint8_t *)0, 0, 1, &color_Black, 0, 0},
};

/// Function table to processing Functions
EffectSM::pEffPrc *const EffectSM::apF_Processors[6] = {
    EffectSM::UpdateBlank, EffectSM::UpdateIdle,    EffectSM::UpdateFreeze,
    EffectSM::UpdateWave,  EffectSM::UpdateRevWave, EffectSM::UpdateFlicker,
};

/**
 * @brief Construct a new Effect State Machine object
 *
 * @param templateLength
 * @param intensity
 * @param crossFade @todo not implemented yet
 */
EffectSM::EffectSM(uint16_t const templateLength, uint8_t const intensity, uint8_t const crossFade) {
    SMIParams.templateLength = templateLength;
    SMIParams.idleIntens     = intensity;
    SMIParams.fadeSteps      = 0;
    SMIParams.dynamicRange   = 30;
    if (crossFade > 0)
        SMIParams.fadeSteps = crossFade; // @todo
    _outputColor = new Color;
}

/**
 * @brief Destroy the Effect State Machine
 * 
 */
    EffectSM::~EffectSM() {
        delete _outputColor;
    }


void EffectSM::SetEffect(EffMacro_t *sequence, Color_t const *startColor, uint8_t initialDelay) {
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
void EffectSM::SetEffect(EffMacro_t *sequence, Color_t const *startColor, const uint8_t *intens,
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
        _p_effMac      = delayPrequel;
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
Color const *EffectSM::Tick(void) {
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

    return apF_Processors[_p_effMac->state](this);
}

/**
 * @brief Sets Waveform index according du desired duration, waveform-length and current step
 */
void EffectSM::SetIndexes(void) {
    SMPValues.waveStep = ((SMIParams.templateLength) << 8);
    SMPValues.waveStep /= _p_effMac->duration;
    SMPValues.waveIdx = (uint16_t)(0 - SMPValues.waveStep);
}

/**
 * @brief Returns fading ramp in cases the color were switched
 * @return uint8_t
 */
/// @todo it seems not te be very clever to delegate this on higher level
uint8_t EffectSM::GetDissolveRatio(void) {
    if (SMIParams.fadeSteps == 0)
        return 0;

    uint8_t dissolving = (uint8_t)((uint16_t)0xFF * SMPValues.dissolveCnt / SMIParams.fadeSteps);
    return dissolving;
}

Color const *EffectSM::UpdateBlank(EffectSM *SM) {
    *(SM->_outputColor) = SM->_curentColor * 0;
    return SM->_outputColor;
}

Color const *EffectSM::UpdateIdle(EffectSM *SM) {
    *(SM->_outputColor) = SM->_curentColor * SM->SMIParams.idleIntens;
    return SM->_outputColor;
}

Color const *EffectSM::UpdateFreeze(EffectSM *SM) {
    // just do nothing and Return currently set color
    return SM->_outputColor;
}

Color const *EffectSM::UpdateWave(EffectSM *SM) {
    EffMacro_t const *const cEffStep = SM->GetStep();
    *(SM->_outputColor) = SM->GetColor() * cEffStep->pWave[SM->GetWaveIdx()] * cEffStep->FsIntensity;
    return SM->_outputColor;
}

Color const *EffectSM::UpdateRevWave(EffectSM *SM) {
    EffMacro_t const *const cEffStep  = SM->GetStep();
    uint8_t                 lastIndex = SM->SMIParams.templateLength - 1;
    *(SM->_outputColor) =
        SM->GetColor() * cEffStep->pWave[lastIndex - SM->GetWaveIdx()] * cEffStep->FsIntensity;
    return SM->_outputColor;
}

Color const *EffectSM::UpdateFlicker(EffectSM *SM) {
    uint8_t k;
    k                   = rand();
    k                   = SM->SMIParams.idleIntens + ((int16_t)k * (SM->SMIParams.dynamicRange) / 0xFF);
    *(SM->_outputColor) = SM->_curentColor * k;
    return SM->_outputColor;
}



















const EffectMacro delaySequence(1,0);

/// Function table to processing Functions
SequenceSM::pEffPrc *const SequenceSM::apF_Processors[6] = {
    SequenceSM::UpdateBlank, SequenceSM::UpdateIdle,    SequenceSM::UpdateFreeze,
    SequenceSM::UpdateWave,  SequenceSM::UpdateRevWave, SequenceSM::UpdateFlicker,
};



SequenceSM::SequenceSM(uint16_t const templateLength, uint8_t const intensity, uint8_t const crossFade) {
    SMIParams.templateLength = templateLength;
    SMIParams.idleIntens     = intensity;
    SMIParams.fadeSteps      = 0;
    SMIParams.dynamicRange   = 30;
    if (crossFade > 0)
        SMIParams.fadeSteps = crossFade; // @todo
    _outputColor = new Color;
}

/**
 * @brief Destroy the Effect State Machine
 * 
 */
    SequenceSM::~SequenceSM() {
        delete _outputColor;
    }


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
Color const *SequenceSM::Tick(void) {
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

    return apF_Processors[_p_effMac->state](this);
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

Color const *SequenceSM::UpdateBlank(SequenceSM *SM) {
    *(SM->_outputColor) = SM->_curentColor * 0;
    return SM->_outputColor;
}

Color const *SequenceSM::UpdateIdle(SequenceSM *SM) {
    *(SM->_outputColor) = SM->_curentColor * SM->SMIParams.idleIntens;
    return SM->_outputColor;
}

Color const *SequenceSM::UpdateFreeze(SequenceSM *SM) {
    // just do nothing and Return currently set color
    return SM->_outputColor;
}

Color const *SequenceSM::UpdateWave(SequenceSM *SM) {
    EffectMacro const *const cEffStep = SM->GetStep();
    *(SM->_outputColor) = SM->GetColor() * cEffStep->pWave[SM->GetWaveIdx()] * cEffStep->FsIntensity;
    return SM->_outputColor;
}

Color const *SequenceSM::UpdateRevWave(SequenceSM *SM) {
    EffectMacro const *const cEffStep  = SM->GetStep();
    uint8_t                 lastIndex = SM->SMIParams.templateLength - 1;
    *(SM->_outputColor) =
        SM->GetColor() * cEffStep->pWave[lastIndex - SM->GetWaveIdx()] * cEffStep->FsIntensity;
    return SM->_outputColor;
}

Color const *SequenceSM::UpdateFlicker(SequenceSM *SM) {
    uint8_t k;
    k                   = rand();
    k                   = SM->SMIParams.idleIntens + ((int16_t)k * (SM->SMIParams.dynamicRange) / 0xFF);
    *(SM->_outputColor) = SM->_curentColor * k;
    return SM->_outputColor;
}


} // namespace Effect