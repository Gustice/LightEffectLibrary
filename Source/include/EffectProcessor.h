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
#include "EffectStateMachine.h"
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
    void SetEffect(EffMacro_t *sequence, Color_t const *sColor = NO_COLOR, uint8_t intens = gu8_idleIntensity);
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





// #define SINGLEEFFECT_COLORWITH 1
// class SingleEffect : public EffectProcessor {
// 	public:
// 	SingleEffect(uint16_t const templateLength);

// 	private:
// 	Color _Color[SINGLEEFFECT_COLORWITH];
// 	Color _ColorOld[SINGLEEFFECT_COLORWITH];
// 	void genImage(Color * color, EffectSM * effStat);
// };

// #define DUALEEFECT_COLORWITH 2
// class DualEffect : public EffectProcessor {
// 	public:
// 	DualEffect(uint16_t const templateLength);

// 	private:
// 	Color _Color[DUALEEFECT_COLORWITH];
// 	Color _ColorOld[DUALEFFECT_COLORWITH];
// 	void genImage(Color * color, EffectSM * effStat);
// };

// class MultiEffect : public EffectProcessor {
// 	public:
// 	MultiEffect(uint8_t u8_Length);

// 	private:
// 	Color * _pColor;
// 	Color * _pColorOld;
// 	void genImage(Color * color, EffectSM * effStat);
// };

} // namespace Effect