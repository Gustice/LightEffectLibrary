

#pragma once

#include <stdint.h>
#include "Color.h"
#include "EffectStateMachine.h"


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
	const uint8_t u8_parts;				// Macro-entries
}EffectList_type;


typedef struct EffektSequenceElement_def {
	const EffMacro_type * element;	// VisualisierungsElement
	uint8_t u8_content;				// Anzahl der eingetragenen Teilschritte
	uint8_t u8_repeats;				// Wiederholungen
}EffSequence_type;
#define COUNT_EFFECT_MAKRO(effect) (sizeof(effect)/sizeof(EffSequence_type))
extern const EffSequence_type em_Idle[];



class EffectProcessor {
	public:
	EffectProcessor(uint16_t const templateLength);
	void SetEffect(EffMacro_type * sequence, Color_t const * sColor = NO_COLOR, uint8_t intens = gu8_idleIntensity);
	void getNextImage(void);
	void SetFlickerRange(uint8_t range);
	
	uint8_t _colorSize;
	Color * _pColor;
	Color * _pColorOld;

	virtual void genImage(Color * color, EffectSM * effStat);
	
	uint8_t u8_fadingCnt;
	uint8_t u8_dissCnt;
	
	EffectSM EffPV;
	EffectSM EffPV_old;
	uint8_t _u8_FlickerRange;
};

#define SINGLEEFFECT_COLORWITH 1
class SingleEffect : public EffectProcessor {
	public:
	SingleEffect(uint16_t const templateLength);
	
	private:
	Color _Color[SINGLEEFFECT_COLORWITH];
	Color _ColorOld[SINGLEEFFECT_COLORWITH];
	void genImage(Color * color, EffectSM * effStat);
};

#define DUALEEFFECT_COLORWITH 2
class DualEffect : public EffectProcessor {
	public:
	DualEffect(uint16_t const templateLength);

	private:
	Color _Color[DUALEEFFECT_COLORWITH];
	Color _ColorOld[DUALEEFFECT_COLORWITH];
	void genImage(Color * color, EffectSM * effStat);
};

// class MultiEffect : public EffectProcessor {
// 	public:
// 	MultiEffect(uint8_t u8_Length);

// 	private:
// 	Color * _pColor;
// 	Color * _pColorOld;
// 	void genImage(Color * color, EffectSM * effStat);
// };

