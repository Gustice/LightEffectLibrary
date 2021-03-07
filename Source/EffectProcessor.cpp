/**
 * @file EffectProcessor.cpp
 * @author Gustice
 * @brief Implementation of Effect-Processor-Class EffectProcessor.h
 * @version 0.1
 * @date 2019-10-03
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "EffectProcessor.h"
#include "EffectBasics.h"
#include "EffectMacro.h"
#include "EffectWaveforms.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

namespace Effect {

EffectProcessor::EffectProcessor(uint16_t const templateLength, uint8_t const fadeSteps)
    : _pColor(), _pColorOld() {
    _EffPV = new EffectSM(templateLength); 
    _EffPV_old = new EffectSM(templateLength);
    _EffPV->SetEffect(eff_Dark, 0);
    _fadeSteps = fadeSteps;
}

EffectProcessor::~EffectProcessor(){
    delete _EffPV;
    delete _EffPV_old;
}

void EffectProcessor::SetEffect(EffMacro_t *sequence, Color_t const *sColor, uint8_t intens) {
    _fadingCnt = _fadeSteps;

    EffectSM * pEsm = _EffPV_old;
    _EffPV_old = _EffPV;
    _EffPV = pEsm;
    _EffPV->SetEffect(sequence, sColor, &intens, 0);
}

Color const * EffectProcessor::Tick(void) {
    uint8_t k;
    _pColor = _EffPV->Tick();

    if (_fadingCnt > 0) { // allways process soft cross dissolve between different macros
        _fadingCnt--;
        k = (uint16_t)_fadingCnt * 0xFF / _fadeSteps;

        _pColorOld = _EffPV_old->Tick();
        crossFadeColors(k);
    // } else { // Process soft cross dissolve between different colors, if needed
    //     uint8_t k = _EffPV.GetDissolveRatio();
    //     if (k > 0) {
    //         crossFadeColors(k);
    //     }
    //     memcpy(&_pColorOld, &_pColor, sizeof(Color));
    }

    return &_pColor;
}

Color *EffectProcessor::crossFadeColors(uint8_t k) {
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

// /************************************************************************/
// /* EffectProcessor 1 LED                                                         */
// /************************************************************************/
// SingleEffect::SingleEffect(uint16_t const templateLength) :
//     EffectProcessor(templateLength) {
// 	_colorSize = 1;
// 	_pColor = _Color;
// 	_pColorOld = _ColorOld;
// }

// void SingleEffect::genImage(Color * color, EffectSM * effStat) {
// 	uint8_t k, kr;

//     // EffMacro_type const * const cEffStep = effStat->GetStep();
// 	// switch (cEffStep->state)
// 	// {
// 	// 	case Light_Idle:
// 	// 	*color = effStat->_aColor * effStat->_idleIntens;
// 	// 	break;

// 	// 	case Light_Wave:
// 	// 	*color = effStat->_aColor * cEffStep->pu8_wave[effStat->ReadTempIdx()] * cEffStep->u8_FSintensity;
// 	// 	break;

// 	// 	case Light_InvWave:
// 	// 	*color = effStat->_aColor * cEffStep->pu8_wave[cu16_TemplateLength -1 - effStat->ReadTempIdx()] *
// cEffStep->u8_FSintensity;
// 	// 	break;

// 	// 	case Light_Flicker:
// 	// 	k = rand();
// 	// 	k =  50 + ((int16_t) k * (50 + _u8_FlickerRange) / 0xFF);
// 	// 	//k = smoothFlick.GetNewAverage(k);
// 	// 	*color = effStat->_aColor * k;
// 	// 	break;

// 	// 	case Light_Sparkle:
// 	// 	kr = ((int16_t)rand() *14 / 0xFF);
// 	// 	k =  50 + kr * kr;
// 	// 	*color = effStat->_aColor * k;
// 	// 	break;

// 	// 	case Light_Blank:
// 	// 	*color = effStat->_aColor * 0;
// 	// 	break;

// 	// 	case Light_Freeze:
// 	// 	default:

// 	// 	break;
// 	// }
// }

// /************************************************************************/
// /* EffectProcessor 2 LED                                                         */
// /************************************************************************/
// DualEffect::DualEffect(uint16_t const templateLength) : EffectProcessor(templateLength) {
// 	_colorSize = 2;
// 	_pColor = _Color;
// 	_pColorOld = _ColorOld;
// }

// void DualEffect::genImage(Color * color, EffectSM * effStat) {
// 	// switch (effStat->_state)
// 	// {
// 	// 	case Light_Idle:
// 	// 	color[0] = color[1] = effStat->_aColor * effStat->_idleIntens;
// 	// 	break;

// 	// 	case Light_Wave:
// 	// 	color[0] = effStat->_aColor * effStat->_p_effMac->pu8_wave[effStat->ReadTempIdx()] *
// effStat->_p_effMac->u8_FSintensity;
// 	// 	color[1] = effStat->_aColor * effStat->_p_effMac->pu8_wave[cu16_TemplateLength -1 - effStat->ReadTempIdx()] *
// effStat->_p_effMac->u8_FSintensity;
// 	// 	break;

// 	// 	case Light_InvWave:
// 	// 	color[0] = effStat->_aColor * effStat->_p_effMac->pu8_wave[cu16_TemplateLength -1 - effStat->ReadTempIdx()] *
// effStat->_p_effMac->u8_FSintensity;
// 	// 	color[1] = effStat->_aColor * effStat->_p_effMac->pu8_wave[effStat->ReadTempIdx()] *
// effStat->_p_effMac->u8_FSintensity;
// 	// 	break;

// 	// 	case Light_Blank:
// 	// 	color[0] = color[1] = effStat->_aColor * 0;
// 	// 	break;

// 	// 	case Light_Flicker:
// 	// 	case Light_Sparkle:
// 	// 	case Light_Freeze:
// 	// 	default:

// 	// 	break;
// 	// }
// }

/************************************************************************/
/* EffectProcessor xx LEDs                                                       */
/************************************************************************/
// MultiEffect::MultiEffect(uint8_t u8_Length) : EffectProcessor() {
// 	_colorSize = u8_Length;
// 	//  @todo pointer to external ColorSource
// }

// void MultiEffect::genImage(Color * color, EffectSM const * effStat) {
// 	uint8_t i;
// 	// switch (effStat->_state)
// 	// {
// 	// 	case Light_Idle:
// 	// 	{
// 	// 		Color col = effStat->_aColor * effStat->_idleIntens;
// 	// 		for (i=0; i < _colorSize; i++)
// 	// 		{
// 	// 			color[i] = col;
// 	// 		}
// 	// 	}
// 	// 	break;

// 	// 	case Light_Wave:
// 	// 	{
// 	// 		uint8_t cStep = effStat->ReadTempIdx();
// 	// 		uint8_t step = cu16_TemplateLength / _colorSize ;

// 	// 		for (i=0; i < _colorSize; i++)
// 	// 		{
// 	// 			color[i] = effStat->_aColor * effStat->_p_effMac->pu8_wave[cStep] * effStat->_p_effMac->u8_FSintensity;
// 	// 			cStep += step;
// 	// 		}
// 	// 	}
// 	// 	break;

// 	// 	case Light_Blank:
// 	// 	{
// 	// 		Color col = effStat->_aColor * 0;
// 	// 		for (i=0; i < _colorSize; i++)
// 	// 		{
// 	// 			color[i] = col;
// 	// 		}
// 	// 	}
// 	// 	break;

// 	// 	case Light_Flicker:
// 	// 	case Light_Sparkle:
// 	// 	case Light_Freeze:
// 	// 	default:

// 	// 	break;
// 	// }
// }

} // namespace Effect
