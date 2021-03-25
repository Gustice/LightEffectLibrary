/**
 * @file Color_t.h
 * @author Gustice
 * @brief   Color-Class Definitions
  *
 * @version 0.6
 * @date 11.03.2021
 *
 * @copyright Copyright (c) 2021
 */

#pragma once
#ifdef __cplusplus 
extern "C"{
#endif

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief Color structure
 * @details Defines red/green/blue and white color channel values
 */
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t white;
} Color_t;

/// Pointer to contant "no wave"
#define NO_WAVE ((uint8_t *)0)
/// Pointer constant to "no color"
#define NO_COLOR ((Color_t *)0)       
/// Used old color agreement (if no color pointer)
#define USEOLD_COLOR NO_COLOR

/// Pointer constant to "no color"
extern const Color_t * pNoColor;

/**
 * @brief Scaling given color self by given factor k
 * @param k scaling factor from 0 to UINT8_MAX
 */
void ScaleColor(Color_t * self, uint8_t k);

/**
 * @brief Mixing given color self with second color by adding each channel
 */
void MixColor(Color_t * self, Color_t c2);

/**
 * @brief Mixing given color self with second color by adding each channel. 
 *  Factor k is applied to color second color and than with (UINT8_MAX - k) to given color.
 * @param k scaling factor from 0 to UINT8_MAX
 */
void MixColorScaled(Color_t * self, Color_t c2, uint8_t k);

/**
 * @brief Test if both colors are equal
 */
bool ColorIsEqual(Color_t * self, Color_t c2);

/**
 * @brief Test if both colors are not equal
 */
bool ColorNotEqual(Color_t * c1, Color_t c2);

/**
 * @brief Set given color to second value
 */
void SetColor(Color_t * self, Color_t c2);

/**
 * @brief Set the Color by RGBW-Values
 */
void SetColorByRgb(Color_t * self, uint8_t r, uint8_t g, uint8_t b, uint8_t w);

/**
 * @brief Write given color as bytestream
 * @details Define USE_OWN_STREAM_CONVERTER to define own converter function
 */
void WriteByteStreamTo(Color_t * self, uint8_t raw[sizeof(Color_t)]);
#undef USE_OWN_STREAM_CONVERTER


/// Black color structure constant
extern const Color_t cBlack;
/// Red color structure constant
extern const Color_t cRed;
/// Green color structure constant
extern const Color_t cGreen;
/// Blue color structure constant
extern const Color_t cBlue;
/// Magenta color structure constant
extern const Color_t cMagenta;
/// Cyan color structure constant
extern const Color_t cCyan;
/// Yellow color structure constant
extern const Color_t cYellow;
/// White color structure constant
extern const Color_t cWhite;
/// Warm white color structure constant (with higher red channel value)
extern const Color_t cWarmWhite;
/// Black color structure constant (with higher blue channel value)
extern const Color_t cColdWhite;
/// Black color structure constant (with higher green channel value)
extern const Color_t cSpookyWhite;

#ifdef __cplusplus
}
#endif