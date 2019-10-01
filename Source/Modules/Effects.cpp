/*
 * Effects.cpp
 *
 * Created: 04.01.2019 22:54:38
 *  Author: Gustice
 */ 

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "EffectWaveforms.h"
#include "Effects.h"


/************************************************************************/
/* Predefined effects                                                   */
/************************************************************************/

// EffMacro_type 
	// state            wave				FS		duration    color	            repeats     next
EffMacro_type eff_Dark[] = {		
	{Light_Idle,	    (uint8_t*)0,		0,		32,	        &color_Black,	    0,	        1},
};

EffMacro_type eff_StartIdle[] = {
	{Light_Wave,	    gau8_initSlope,		0xFF,	16,         &color_ColdWhite,   0,          1},
	{Light_Idle,	    (uint8_t*)0,		0xFF,	32,         USEOLD_COLOR,		0,          1},
};

EffMacro_type eff_Idle[] = {
	{Light_Idle,	    (uint8_t*)0,		0xFF,	32,         &color_ColdWhite,	0,          0},
};

EffMacro_type eff_StdPulse[] = {
	{Light_Idle,	    (uint8_t*)0,		0,		32,         &color_ColdWhite,	0,          1},
	{Light_Wave,	    gau8_offsetPulse,	0xFF,	32,         USEOLD_COLOR,		0,          2},
	{Light_Idle,	    (uint8_t*)0,		0,		32,         USEOLD_COLOR,		0,          0},
};

EffMacro_type eff_NervousPulse[] = {
	{Light_Idle,	    (uint8_t*)0,		0,		4,         &color_ColdWhite,	0,          1},
	{Light_Wave,	    gau8_offsetPulse,	0xFF,	8,         USEOLD_COLOR,		0,          2},
	{Light_Idle,	    (uint8_t*)0,		0,		4,         USEOLD_COLOR,		0,          0},
};


EffMacro_type eff_StdWipe[] = {
	{Light_Idle,	    (uint8_t*)0,		0,		32,         &color_ColdWhite,	0,          1},
		
	{Light_Wave,	    gau8_offsetSlope,	0xFF,	32,         USEOLD_COLOR,		0,          2},
	{Light_Freeze,	    (uint8_t*)0,		0,		32,         USEOLD_COLOR,		0,          3},
	{Light_InvWave,	    gau8_offsetSlope,	0xFF,	32,         USEOLD_COLOR,		0,          4},
	{Light_Freeze,	    (uint8_t*)0,		0,		32,         USEOLD_COLOR,		0,          1},
};

EffMacro_type eff_AsymPulse[] = {
	{Light_Blank,	    (uint8_t*)0,		0,		16,         &color_ColdWhite,	0,          1},
		
	{Light_Wave,	    gau8_fullPulse,		0xFF,	16,         USEOLD_COLOR,		0,          2},
	{Light_Blank,	    (uint8_t*)0,		0,		4,         USEOLD_COLOR,		0,          3},
	{Light_Blank,	    (uint8_t*)0,		0,		32,         USEOLD_COLOR,		0,          1},
};

EffMacro_type eff_AsymPulseInv[] = {
	{Light_Blank,	    (uint8_t*)0,		0,		16,         &color_ColdWhite,	0,          1},
	
	{Light_Blank,	    (uint8_t*)0,		0,		4,         USEOLD_COLOR,		0,          2},
	{Light_Wave,	    gau8_fullPulse,		0xFF,	16,         USEOLD_COLOR,		0,          3},
	{Light_Blank,	    (uint8_t*)0,		0,		32,         USEOLD_COLOR,		0,          1},
};


EffMacro efm_StdPulse[] = {
	EffMacro(Light_Idle,	    (uint8_t*)0,		0,		32,         &_ctColdWhite,		0,          1),
	EffMacro(Light_Wave,	    gau8_offsetPulse,	0xFF,	32,         USEOLD_COLOR_OBJ,	0,          2),
	EffMacro(Light_Idle,	    (uint8_t*)0,		0,		32,         USEOLD_COLOR_OBJ,	0,          0),
};




/************************************************************************/
/* Effect base type                                                     */
/************************************************************************/
const uint8_t cu8_stdFading = 15;

Effect::Effect(void) {
	EffPV.SetEffect(eff_Dark, 0);
	_u8_FlickerRange = 30;
}

void Effect::SetEffect(EffMacro_type * sequence, Color_t const * sColor, uint8_t intens) {
	u8_fadingCnt = cu8_stdFading;

	memcpy(&EffPV_old, &EffPV, sizeof(EffPV));
	EffPV.SetEffect(sequence, sColor, intens);
}


void Effect::getNextImage(void) {
	uint8_t k,i;
	EffPV.Tick();
	genImage(_pColor, &EffPV);

	if (u8_fadingCnt > 0)
	{ // allways process soft cross dissolve between different macros
		u8_fadingCnt--;
		k = (uint16_t)u8_fadingCnt*0xFF/cu8_stdFading;

		EffPV_old.Tick();
		genImage(_pColorOld, &EffPV_old);
		for (i=0; i<_colorSize; i++)
		{
			_pColorOld[i] = _pColorOld[i] * k;
			_pColor[i] = _pColor[i] * (0xFF-k);
			_pColor[i] = _pColor[i] +_pColorOld[i];
		}
	}
	else
	{ // Process soft cross dissolve between different colors, if needed
		uint8_t u8_dissCnt = EffPV.ProceedDissolve();
		if (u8_dissCnt > 0)
		{
			k = (uint16_t)u8_dissCnt*0xFF/cu8_stdFading;
			for (i=0; i<_colorSize; i++)
			{
				_pColorOld[i] = _pColorOld[i] * k;
				_pColor[i] = _pColor[i] * (0xFF-k);
				_pColor[i] = _pColor[i] +_pColorOld[i];
			}
		}
		memcpy(_pColorOld,_pColor,sizeof(Color));
	}
}

void Effect::SetFlickerRange(uint8_t range)
{
	
}

void Effect::genImage(Color * color, EffectSM * effStat)
{
	// should be pure virtual but compiler doesn't accept it somehow
}


/************************************************************************/
/* Effect 1 LED                                                         */
/************************************************************************/
SingleEffect::SingleEffect(void) : Effect() {
	_colorSize = 1;
	_pColor = _Color;
	_pColorOld = _ColorOld;
}

void SingleEffect::genImage(Color * color, EffectSM * effStat) {
	uint8_t k, kr;
	switch (effStat->_state)
	{
		case Light_Idle:
		*color = effStat->_aColor * effStat->_idleIntens;
		break;
				
		case Light_Wave:
		*color = effStat->_aColor * effStat->_p_effMac->pu8_wave[effStat->ReadTempIdx()] * effStat->_p_effMac->u8_FSintensity;
		break;

		case Light_InvWave:
		*color = effStat->_aColor * effStat->_p_effMac->pu8_wave[cu16_TemplateLength -1 - effStat->ReadTempIdx()] * effStat->_p_effMac->u8_FSintensity;
		break;

		case Light_Flicker:
		k = rand();
		k =  50 + ((int16_t) k * (50 + _u8_FlickerRange) / 0xFF);
		//k = smoothFlick.GetNewAverage(k);
		*color = effStat->_aColor * k;
		break;

		case Light_Sparkle:
		kr = ((int16_t)rand() *14 / 0xFF);
		k =  50 + kr * kr;
		*color = effStat->_aColor * k;
		break;

		case Light_Blank:
		*color = effStat->_aColor * 0;
		break;

		case Light_Freeze:
		default:

		break;
	}
}


/************************************************************************/
/* Effect 2 LED                                                         */
/************************************************************************/
DualEffect::DualEffect(void) : Effect() {
	_colorSize = 2;
	_pColor = _Color;
	_pColorOld = _ColorOld;
}

void DualEffect::genImage(Color * color, EffectSM * effStat) {
	switch (effStat->_state) 
	{
		case Light_Idle:
		color[0] = color[1] = effStat->_aColor * effStat->_idleIntens;
		break;
		
		case Light_Wave:
		color[0] = effStat->_aColor * effStat->_p_effMac->pu8_wave[effStat->ReadTempIdx()] * effStat->_p_effMac->u8_FSintensity;
		color[1] = effStat->_aColor * effStat->_p_effMac->pu8_wave[cu16_TemplateLength -1 - effStat->ReadTempIdx()] * effStat->_p_effMac->u8_FSintensity;
		break;

		case Light_InvWave:
		color[0] = effStat->_aColor * effStat->_p_effMac->pu8_wave[cu16_TemplateLength -1 - effStat->ReadTempIdx()] * effStat->_p_effMac->u8_FSintensity;
		color[1] = effStat->_aColor * effStat->_p_effMac->pu8_wave[effStat->ReadTempIdx()] * effStat->_p_effMac->u8_FSintensity;
		break;
		
		case Light_Blank:
		color[0] = color[1] = effStat->_aColor * 0;
		break;
		
		case Light_Flicker:
		case Light_Sparkle:
		case Light_Freeze:
		default:

		break;
	}
}

/************************************************************************/
/* Effect xx LEDs                                                       */
/************************************************************************/
MultiEffect::MultiEffect(uint8_t u8_Length) : Effect() {
	_colorSize = u8_Length;
	//  @todo pointer to external ColorSource
}

void MultiEffect::genImage(Color * color, EffectSM * effStat) {
	uint8_t i;
	switch (effStat->_state)
	{
		case Light_Idle:
		{
			Color col = effStat->_aColor * effStat->_idleIntens;
			for (i=0; i < _colorSize; i++)
			{
				color[i] = col;
			}
		}
		break;

		case Light_Wave:
		{
			uint8_t cStep = effStat->ReadTempIdx();
			uint8_t step = cu16_TemplateLength / _colorSize ;
			
			for (i=0; i < _colorSize; i++)
			{
				color[i] = effStat->_aColor * effStat->_p_effMac->pu8_wave[cStep] * effStat->_p_effMac->u8_FSintensity;
				cStep += step;
			}
		}
		break;
		
		case Light_Blank:
		{
			Color col = effStat->_aColor * 0;
			for (i=0; i < _colorSize; i++)
			{
				color[i] = col;
			}
		}
		break;
		
		case Light_Flicker:
		case Light_Sparkle:
		case Light_Freeze:
		default:

		break;
	}
}


