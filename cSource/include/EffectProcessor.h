#pragma once
#ifdef __cplusplus 
extern "C"{
#endif

#include "Color.h"
#include "EffectStateMachine.h"
#include <stdint.h>

extern const uint8_t gu8_idleIntensity; // @todo this is bad

class EffectProcessor {
  public:
    EffectProcessor(uint16_t const templateLength, uint8_t const fadeSteps);
    ~EffectProcessor();
    void SetEffect(EffMacro_t *sequence, color_t const *sColor = NO_COLOR, uint8_t intens = gu8_idleIntensity);
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

#ifdef __cplusplus
}
#endif