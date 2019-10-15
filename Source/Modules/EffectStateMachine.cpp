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
#include <math.h>
#include <stdlib.h>

namespace Effect {

/// Optional prequel for delayed start.
/// \li Duration must be != 0
/// \li Next entry will be first entry in given Effect sequence
EffMacro_type delayPrequel[] = {
    {Light_Blank, (uint8_t *)0, 0, 1, &color_Black, 0, 0},
};

EffectSM::pEffPrc * const EffectSM::apF_Processors[6] = {
    EffectSM::UpdateBlank,
    EffectSM::UpdateIdle,
    EffectSM::UpdateFreeze,
    EffectSM::UpdateWave,
    EffectSM::UpdateWave,
    EffectSM::UpdateFlicker,
};

/**
 * @brief Construct a new Effect State Machine object
 *
 * @param templateLength
 * @param intensity
 * @param crossFade
 */
EffectSM::EffectSM(uint16_t const templateLength, uint8_t const intensity, uint8_t const crossFade) {
    _u16_templateLength = templateLength;
    _u8_idleIntens      = intensity;
    _u8_fadeSteps       = 1;
    if (crossFade > 0)
        _u8_fadeSteps = crossFade;
}

void EffectSM::SetEffect(EffMacro_type *sequence, Color_t const *startColor, uint8_t intialDelay) {
    SetEffect(sequence, startColor, intialDelay, _u8_idleIntens);
}

/**
 * @brief Sets the effect state machine to process given effect macro
 *
 * @param sequence
 * @param startColor
 * @param delayedStart
 * @param intens
 */
void EffectSM::SetEffect(EffMacro_type *sequence, Color_t const *startColor, uint8_t delayedStart, uint8_t intens) {
    if (startColor == NO_COLOR) {
        _curentColor.SetColor(*sequence->color);
    } else {
        _curentColor.SetColor(*startColor);
    }
    _u8_idleIntens = intens;

    _p_effSeq = sequence;
    if (delayedStart > 0) {
        _p_effMac = delayPrequel;
        _u8_tick  = delayedStart;
    } else {
        _p_effMac = sequence;
        _u8_tick  = _p_effMac->u8_duration;
    }

    _u8_dissolveCnt = 0;
    _u8_repeats     = _p_effMac->u8_repeats;
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
Color const * EffectSM::Tick(void) {
    // tick-increment
    if (_u8_tick-- == 0) {
        // repeats-decrement
        if (_u8_repeats-- == 0) {
            _p_effMac   = &(_p_effSeq[_p_effMac->next]);
            _u8_repeats = _p_effMac->u8_repeats;

            // execute color change if necessary
            if (_p_effMac->color != NO_COLOR) {
                _u8_dissolveCnt = _u8_fadeSteps;
                _curentColor.SetColor(*_p_effMac->color);
            }
        }

        _u8_tick = _p_effMac->u8_duration;
        this->SetIndexes();
    }

    _u16_waveIdx += _u16_waveStep;
    if (_u8_dissolveCnt > 0) {
        _u8_dissolveCnt--;
    }

    return apF_Processors[_p_effMac->state](this);
}

void EffectSM::SetIndexes(void) {
    _u16_waveStep = (_u16_templateLength << 8) / _p_effMac->u8_duration;
    _u16_waveIdx  = (uint16_t)(0 - _u16_waveStep);
}

uint8_t EffectSM::GetDissolveRatio(void) {
    uint8_t dissolving = (uint8_t)((uint16_t)0xFF * _u8_dissolveCnt / _u8_fadeSteps);
    return dissolving;
}

    Color const * EffectSM::UpdateBlank(EffectSM * SM){
        *(SM->_outputColor) = SM->_curentColor * 0;
        return SM->_outputColor;
    }

    Color const * EffectSM::UpdateIdle(EffectSM * SM){
        *(SM->_outputColor) = SM->_curentColor * SM->_u8_idleIntens;
        return SM->_outputColor;
    }
    
    Color const * EffectSM::UpdateFreeze(EffectSM * SM){
        // just do nothing
        return SM->_outputColor;
    }

    Color const * EffectSM::UpdateWave(EffectSM * SM){
            // EffMacro_type const *const cEffStep = effStat->GetStep();
        // *color = effStat->GetColor() * cEffStep->pu8_wave[effStat->GetWaveformIdx()] * cEffStep->u8_FSintensity;
        return SM->_outputColor;
    }

    Color const * EffectSM::UpdateRevWave(EffectSM * SM){
            // EffMacro_type const *const cEffStep = effStat->GetStep();
        // *color = effStat->GetColor() * cEffStep->pu8_wave[cu16_TemplateLength - 1 - effStat->GetWaveformIdx()] * cEffStep->u8_FSintensity;
        return SM->_outputColor;
    }

    Color const * EffectSM::UpdateFlicker(EffectSM * SM){
        uint8_t k;
        k      = rand();
        k      = SM->_u8_idleIntens + ((int16_t)k * (SM->_u8_idleIntens + SM->_u8_dynamicRange) / 0xFF);
        *(SM->_outputColor) = SM->_curentColor * k;
        return SM->_outputColor;
    }

} // namespace Effect