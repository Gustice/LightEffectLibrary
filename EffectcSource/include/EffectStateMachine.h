/**
 * @file EffectStateMachine.h
 * @author Gustice
 * @brief Definition of Effect-State-Machine
 * @version 0.6
 * @date 2021-03-23
 *
 * @copyright Copyright (c) 2021
 */

#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "EffectBasics.h"
#include "color_t.h"
#include <stdint.h>

/// Collection of process values to maintain processing state machine
typedef struct {
    /// Decrementing tick, counting duration
    uint8_t tick;
    /// Decrementing repeats of current Effect-part
    uint8_t repeats;
    /// Dissolve counter dissolves the leading color each tick
    uint8_t dissolveCnt;
    /// Index to macro element
    uint8_t macroIdx;
    /// Current index to waveform
    uint16_t waveIdx;
    /// Index step each tick according to duration and template length.
    uint16_t waveStep;
} SM_ProcessValues_t;

/// Collection of object configuration at statup
typedef struct {
    /// Idle intensity as background color
    uint8_t idleIntens;
    /// dynamic range as variation around background color
    uint8_t dynamicRange;
    /// Fade steps for color changes
    uint8_t fadeSteps;
    /// Waveform template length -> index steps are calculated accordingly by give duration
    uint16_t templateLength;
} SM_ParameterValues_t;

/**
 * @brief Effect State Machine Class
 * @details Processes Effect macro array macro entry subsequently
 * Processing is executed in following manner:
 *  \li Index-incrementing each Tick
 *      \li ticks until duration \ref EffMacro is finished
 *      \li each tick the index incremented by templateLength/duration
 *  \li Repeats-processing
 *      \li Each repeat the tick and the index are set back to start
 *      \li Switches to next Macro entry, that is given by 'next' index
 *      \li Color changes can be executed optionally
 */
typedef struct EffectSM_def {
    /// Concentrated parameter values of instance
    SM_ParameterValues_t SMIParams;
    /// Concentrated process values of instance
    SM_ProcessValues_t SMPValues;

    /// First element of Effect macro stack
    EffMacro_t const *p_effSeq;
    /// Currently indexed Effect-part
    EffMacro_t const *p_effMac;

    /// Reference to output color
    Color_t *outputColor;

    /// Last color
    Color_t lastColor;

    /// Current color
    /// \li Either forced by start with parameter
    /// \li Or given by current Effect-part
    Color_t curentColor;
} EffectSM_t;

/**
 * @brief Prepare Effect-State-Machine object
 * @param self Object reference
 * @param param Parameter set for the instance
 * @param color Reference to color container
 */
void SM_Construct(EffectSM_t *self, SM_ParameterValues_t param, Color_t *color);

/**
 * @brief Get the index for the current waveform
 * @details The index is calculated with a higher accuracy in the background.
 * @param self Object reference
 * @return index to waveform position
 */
const uint8_t SM_GetWaveIdx(EffectSM_t *self);

/**
 * @brief Set the dynamic range of effects
 *
 * @param self object reference
 * @param range Range may be from 0 to (UINT8_MAX-Idle)
 */
void SM_SetDynamicRange(EffectSM_t *self, uint8_t range);

/**
 * @brief Execute step of state machine
 * @param self object reference
 * @return Reference to color result
 */
Color_t const *SM_Tick(EffectSM_t *self);

/**
 * @brief Set next effect sequence
 * @param self Object reference
 * @param sequence reference to sequence that has to be processed next
 * @param startColor Start color. Use NO_COLOR to start with default color
 * @param intens Idle intensity for effect. Use NULL to start with default intensity
 * @param delayedStart Number of steps that are waited before effect is started
 */
void    SM_SetEffect(EffectSM_t *self, EffMacro_t *sequence, Color_t const *startColor, const uint8_t *intens,
                     const uint8_t delayedStart);

#ifdef __cplusplus
}
#endif