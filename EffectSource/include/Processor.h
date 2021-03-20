/**
 * @file EffectProcessor.h
 * @author Gustice
 * @brief Effect-Processor-Class for processing effect scenarios and switching actions
 * @version 0.1
 * @date 2019-10-03
 *
 * @copyright Copyright (c) 2019
 *
 */
#pragma once

#include "Color.h"
#include "StateMachine.h"
#include <stdint.h>

namespace Effect {

extern const uint8_t gu8_idleIntensity; // @todo this is bad

/**
 * @brief EffectProcessor Class
 * @details Maintains State-Machines, Processes effect-switching, and invokes generation
 *  of raw data stream for LEDs.
 *      \li If effect scenarios are changed a crossfading is executed
 *      \li Rawdatastream for each Color object is generated
 */
class EffectProcessor {
  public:
    EffectProcessor(uint16_t const templateLength, uint8_t const fadeSteps);
    ~EffectProcessor();
    void SetEffect(Macro_t *sequence, color_t const *sColor = noColor, uint8_t intens = gu8_idleIntensity);
    Color const * Tick(void);

  private:
    uint8_t _colorSize;
    Color   _pColor;
    Color   _pColorOld;
    uint8_t _fadeSteps;
    uint8_t _fadingCnt;
    EffectSM * _EffPV;
    EffectSM * _EffPV_old;

    Color *crossFadeColors(uint8_t k);
    EffectProcessor();
};

} // namespace Effect