

#pragma once

#include <stdint.h>
#include "Color.h"

extern const uint16_t cu16_TemplateLength;
extern const uint8_t gu8_idleIntensity;
enum e_EffectPart {
	Light_Idle = 0,
	Light_Wave,
	Light_InvWave,
	Light_Flickr,
	Light_Sparkle,
	Light_Freeze,
	Light_Blank,

	Light_States,
};

typedef const struct EffectMacroElement_def {
	e_EffectPart state;			// SubEffect
	uint8_t const * pu8_wave;	// waveform reference
	uint8_t u8_FSintensity;		// Full-Scale-intensity
	uint8_t u8_duration;		// Effect duration
	const Color_type * color;	// SubEffect color
	int8_t u8_repeats;			// Num of Repeats
	int8_t next;				// Next SubEffect 
} EffMacro_type;

class EffMacro {
    public:
    EffMacro(
		e_EffectPart state,
    	uint8_t const * pu8_wave,
    	uint8_t u8_FSintensity,
    	uint8_t u8_duration,
    	Color const * color,
    	int8_t u8_repeats,
    	int8_t next
    );

	e_EffectPart _state;		// SubEffect
	uint8_t const * _pu8_wave;	// waveform reference
	uint8_t _u8_FSintensity;	// Full-Scale-intensity
	uint8_t _u8_duration;		// Effect duration
	Color const * _color;	    // SubEffect color
	int8_t _u8_repeats;			// Num of Repeats
	int8_t _next;				// Next SubEffect 
};
class DualEffMacro : EffMacro {
	public:
    DualEffMacro(
		e_EffectPart state,
		e_EffectPart stateI,
    	uint8_t const * pu8_wave,
    	uint8_t const * pu8_waveI,
    	uint8_t u8_FSintensity,
    	uint8_t u8_duration,
    	Color const * color,
		Color const * colorI,
    	int8_t u8_repeats,
    	int8_t next
    ); // : EffMacro(state, pu8_wave, u8_FSintensity, u8_duration, color, u8_repeats, next);

	e_EffectPart _stateI;		// SubEffect
	uint8_t const * _pu8_waveI;	// waveform reference
	Color const * _colorI;	    // SubEffect color
};

class EffectMemory {
	friend class Effect;
	friend class SingleEffect;
	friend class DualEffect;
	friend class MultiEffect;

	public:
	void SetEffect(EffMacro_type * sequence, Color_type const * sColor = NO_COLOR, uint8_t intens = gu8_idleIntensity);
	//void SetEffect(EffMacro_type * sequence, Color const * sColor = NO_COLOR_OBJ, uint8_t intens = gu8_idleIntensity);
	void Tick(void);
	uint8_t ProceedDissolve (void);
	
	private: // @todo change to protected an thest reslts
	EffMacro_type const * _p_effSeq;
	EffMacro_type const * _p_effMac;
		
	uint8_t _u8_tick;
	uint8_t _u8_tickEnd;
	
	e_EffectPart _state;
	uint16_t _u16_templateIdx;
	uint16_t _u16_templateStep;
 	uint8_t _u8_repeats;
	
	uint8_t _u8_dissCnt;
	Color _aColor;
	uint8_t _idleIntens;
	Color_type const * _fColor;
	
	void SetIndexes(void);
	uint8_t ReadTempIdx(void);
};

extern const uint8_t u8_EffectsCount;
extern const uint8_t gau8_offsetPulse[];
extern const uint8_t gau8_initSlope[];
extern const uint8_t gau8_offsetSlope[];
extern const uint8_t gau8_fullSlope[];
extern const uint8_t gau8_fullPulse[];
extern const uint8_t gu8_idleIntensity;
