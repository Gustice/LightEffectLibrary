#include "EffectSequencer.h"
#include "EffectBasics.h"
#include "EffectMacro.h"
#include "EffectWaveforms.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

namespace Effect {


EffectSequencer::EffectSequencer(uint16_t const templateLength, uint8_t const fadeSteps)
    : _pColor(), _pColorOld() {
    _EffPV = new SequenceSM(templateLength, 1); 
    _EffPV_old = new SequenceSM(templateLength, 1);
    _EffPV->SetEffect(StdDark, 0);
    _fadeSteps = fadeSteps;
}

EffectSequencer::~EffectSequencer(){
    delete _EffPV;
    delete _EffPV_old;
}

void EffectSequencer::SetEffect(EffectMacro *sequence, Color_t const *sColor, uint8_t intens) {
    _fadingCnt = _fadeSteps;

    SequenceSM * pEsm = _EffPV_old;
    _EffPV_old = _EffPV;
    _EffPV = pEsm;
    _EffPV->SetEffect(sequence, sColor, &intens, 0);
}

Color const * EffectSequencer::Tick(void) {
    uint8_t k;
    _pColor = _EffPV->Tick();

    if (_fadingCnt > 0) { // allways process soft cross dissolve between different macros
        _fadingCnt--;
        k = (uint16_t)_fadingCnt * 0xFF / _fadeSteps;

        _pColorOld = _EffPV_old->Tick();
        crossFadeColors(k);
    // } else { // Process soft cross dissolve between different colors, if needed
    //     uint8_t k = _EffP    V.GetDissolveRatio();
    //     if (k > 0) {
    //         crossFadeColors(k);
    //     }
    //     memcpy(&_pColorOld, &_pColor, sizeof(Color));
    }

    return &_pColor;
}

Color *EffectSequencer::crossFadeColors(uint8_t k) {
    // int i;
    // for (i = 0; i < _colorSize; i++) {
    //     _pColorOld[i] = _pColorOld[i] * k;
    //     _pColor[i]    = _pColor[i] * (0xFF - k);
    //     _pColor[i]    = _pColor[i] + _pColorOld[i];
    // }

    _pColorOld = _pColorOld * k;
    _pColor    = _pColor * (0xFF - k);
    _pColor    = _pColor + _pColorOld;

    return &_pColor;
}


} // namespace Effect