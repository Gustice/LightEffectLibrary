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

/// Optional prequel for delayed start. 
/// \li Duration must be != 0 
/// \li Next entry will be first entry in given Effect sequence
EffMacro_type delayPrequel[] = {
	{Light_Blank, (uint8_t*)0, 0, 1, &color_Black, 0, 0},
};


EffectSM::EffectSM(uint16_t const templateLength, uint8_t const intensity, uint8_t const crossFade) {
    _u8_idleIntens = intensity;
    _u8_fadeSteps = crossFade;
    _u16_templateLength = templateLength;
}

void EffectSM::SetEffect(EffMacro_type *sequence, Color_t const *startColor, uint8_t intialDelay) {
    SetEffect(sequence, startColor,intialDelay, _u8_idleIntens);
}

void EffectSM::SetEffect(EffMacro_type *sequence, Color_t const *startColor, uint8_t delayedStart, uint8_t intens) {
    if (startColor == NO_COLOR) {
        _curentColor.SetColor(*sequence->color);
    } else {
        _curentColor.SetColor(*startColor);
    }
    _u8_idleIntens = intens;

    _p_effSeq   = sequence;
    if (delayedStart > 0) {
        _p_effMac =  delayPrequel;
        _u8_tick = delayedStart;
    }
    else {
        _p_effMac   = sequence;
        _u8_tick    = _p_effMac->u8_duration;
    }
    
    _u8_dissolveCnt = 0;
    _u8_repeats = _p_effMac->u8_repeats;
    this->SetIndexes();
}

uint8_t EffectSM::Tick(void) {
    // tick-increment
    if (_u8_tick-- == 0) {
        // repeats-decrement
        if (_u8_repeats-- == 0) {
            _p_effMac = &(_p_effSeq[_p_effMac->next]);
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
    return _u8_tick;
}

void EffectSM::SetIndexes(void) {
    _u16_waveStep = (_u16_templateLength << 8) / _p_effMac->u8_duration;
    _u16_waveIdx  = (uint16_t)(0 - _u16_waveStep);
}

uint8_t EffectSM::GetDissolveRatio(void) {
    uint8_t dissolving = (uint8_t)((uint16_t)0xFF*_u8_dissolveCnt/_u8_fadeSteps);
    return dissolving;
}
