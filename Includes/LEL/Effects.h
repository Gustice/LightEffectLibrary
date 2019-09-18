

#pragma once

#include <stdint.h>
#include "Color.h"
#include "EffectMemory.h"


#define COUNT_EFFECT_ELEMENT(effect) (sizeof(effect)/sizeof(EffMacro_type))

extern EffMacro_type eff_Dark[];
extern EffMacro_type eff_StartIdle[];
extern EffMacro_type eff_Idle[];
extern EffMacro_type eff_StdPulse[];
extern EffMacro_type eff_NervousPulse[];
extern EffMacro_type eff_StdWipe[];
extern EffMacro_type eff_AsymPulse[];
extern EffMacro_type eff_AsymPulseInv[];
extern EffMacro_type eff_ShiftAsymPulse[];
extern EffMacro_type eff_ShiftAsymPulseInv[];

typedef struct EffectList_def {
	const EffMacro_type * Macro;		// Macro reference
	const uint8_t u8_parts;				// Makro-Eintr�ge
}EffectList_type;


typedef struct EffektSequenceElement_def {
	const EffMacro_type * element;	// VisualisierungsElement
	uint8_t u8_content;				// Anzahl der eingetragenen Teilschritte
	uint8_t u8_repeats;				// Wiederholungen
}EffSequence_type;
#define COUNT_EFFECT_MAKRO(effect) (sizeof(effect)/sizeof(EffSequence_type))
extern const EffSequence_type em_Idle[];



class Effect {	
	public:
	Effect(void);
	void SetEffect(EffMacro_type * sequence, Color_type const * sColor = NO_COLOR, uint8_t intens = gu8_idleIntensity);
	void getNextImage(void);
	void SetFlickrRange(uint8_t range);
	
	uint8_t _colorSize;
	Color * _pColor;
	Color * _pColorOld;

	virtual void genImage(Color * color, EffectMemory * effStat);
	
	uint8_t u8_fadingCnt;
	uint8_t u8_dissCnt;
	
	EffectMemory EffPV;
	EffectMemory EffPV_old;
	uint8_t _u8_FlickrRange;
};

#define SINGLEEFFECT_COLORWITH 1
class SingleEffect : public Effect {
	public:
	SingleEffect(void);
	
	private:
	Color _Color[SINGLEEFFECT_COLORWITH];
	Color _ColorOld[SINGLEEFFECT_COLORWITH];
	void genImage(Color * color, EffectMemory * effStat);
};

#define DUALEEFFECT_COLORWITH 2
class DualEffect : public Effect {
	public:
	DualEffect(void);

	private:
	Color _Color[DUALEEFFECT_COLORWITH];
	Color _ColorOld[DUALEEFFECT_COLORWITH];
	void genImage(Color * color, EffectMemory * effStat);
};

class MultiEffect : public Effect {
	public:
	MultiEffect(uint8_t u8_Length);

	private:
	Color * _pColor;
	Color * _pColorOld;
	void genImage(Color * color, EffectMemory * effStat);
};

