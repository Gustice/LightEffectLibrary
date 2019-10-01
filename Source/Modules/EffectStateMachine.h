/**
 * @file EffectStateMachine.h
 * @author Gustice
 * @brief Effect-State-Machine-Class for processing Effect Macros
 * @version 0.1
 * @date 2019-10-01
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once

#include <stdint.h>
#include "Color.h"
#include "EffectMacro.h"

extern const uint16_t cu16_TemplateLength; // @todo this is bad
extern const uint8_t gu8_idleIntensity; // @todo this is bad

/**
 * @brief Effect State Machine Class
 * @details Processes Effect macro array Macro entry subsequently
 * Processing is executed in following manner:
 *  \li Index-incrementing each Tick
 *      \li ticks until duration \ref EffMacro is finished
 *      \li each tick the index incremented by templateLenght/duration
 *  \li Repeats-processing 
 *      \li Each repeat the tick and the index are set back to start
 *      \li Switches to next Macro enty
 *      \li Next entry is given by 'next' index
 *      \li Color changes can be executed optionally
 */
class EffectSM {
	public:
	void SetEffect(EffMacro_type * sequence, Color_t const * sColor = NO_COLOR, uint8_t intens = gu8_idleIntensity);
	//void SetEffect(EffMacro_type * sequence, Color const * sColor = NO_COLOR_OBJ, uint8_t intens = gu8_idleIntensity);
	void Tick(void);
	uint8_t ProceedDissolve (void);
	
	private: // @todo change to protected and tests results
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
	Color_t const * _fColor;
	
	void SetIndexes(void);
	uint8_t ReadTempIdx(void);
};

