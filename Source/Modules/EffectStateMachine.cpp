/**
 * @file EffectStateMachine.cpp
 * @author Gustice
 * @brief Implementation of EffectStateMachine-Class EffectStateMachine.h
 * @version 0.1
 * @date 2019-10-01
 *
 * @copyright Copyright (c) 2019
 *
 */
#include "EffectStateMachine.h"

const uint8_t cu8_Fading = 5;

void EffectSM::SetEffect(EffMacro_type *sequence, Color_t const *sColor, uint8_t intens) {
    if (sColor == NO_COLOR) {
        _aColor.SetColor(*sequence->color);
        _fColor = NO_COLOR;
    } else {
        _aColor.SetColor(*sColor);
        _fColor = sColor;
    }
    _idleIntens = intens;

    _p_effSeq   = sequence;
    _p_effMac   = sequence;
    _u8_tick    = 0;
    _u8_tickEnd = _p_effMac->u8_duration;
    _u8_repeats = _p_effMac->u8_repeats;

    this->SetIndexes();
}

void EffectSM::Tick(void) {
    // tick-increment
    if (++_u8_tick >= _u8_tickEnd) {
        _u8_tick = 0;

        // repeats-decrement
        if (_u8_repeats-- == 0) {
            _p_effMac = &(_p_effSeq[_p_effMac->next]);
            _u8_repeats = _p_effMac->u8_repeats;

            // execute color change if necessary
            if (_p_effMac->color != NO_COLOR) {
                _u8_dissCnt = cu8_Fading;
                if (_fColor == NO_COLOR) {
                    _aColor.SetColor(*_p_effMac->color);
                }
            }
        }

        _u8_tickEnd = _p_effMac->u8_duration;
        this->SetIndexes();
    }

    _u16_templateIdx += _u16_templateStep;
}

void EffectSM::SetIndexes(void) {
    _u16_templateStep = (cu16_TemplateLength << 8) / _u8_tickEnd;
    _u16_templateIdx  = (uint16_t)(0 - _u16_templateStep);
}

uint8_t EffectSM::ReadTempIdx(void) { return (_u16_templateIdx >> 8); }

uint8_t EffectSM::ProceedDissolve(void) {
    if (_u8_dissCnt > 0) {
        _u8_dissCnt--;
    }
    return _u8_dissCnt;
}
