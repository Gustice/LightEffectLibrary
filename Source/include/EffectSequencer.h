/**
 * @file EffectSequencer.h
 * @author Gustice
 * @brief Effect-Processor-Class for processing effect scenarios and switching actions
 * @version 0.1
 * @date 2021-03-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include "Color.h"
#include "SequenceStateMachine.h"
#include <stdint.h>


namespace Effect {


class EffectSequencer {
  public:
    EffectSequencer(uint16_t const templateLength, uint8_t const fadeSteps);
    ~EffectSequencer();
    void SetEffect(EffectMacro *sequence, Color_t const *sColor = NO_COLOR, uint8_t intens = gu8_idleIntensity);
    Color const * Tick(void);

  private:
    uint8_t _colorSize;
    Color   _pColor;
    Color   _pColorOld;
    uint8_t _fadeSteps;
    uint8_t _fadingCnt;
    SequenceSM * _EffPV;
    SequenceSM * _EffPV_old;

    Color *crossFadeColors(uint8_t k);
    EffectSequencer();
};

} // namespace Effect