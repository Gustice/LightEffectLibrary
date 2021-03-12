/**
 * @file color_t.h
 * @author Gustice
 * @brief   Color-Class Definitions
  *
 * @version 0.1
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
 * @brief Color Structure
 * @details Defines red/green/blue and white color channel values
 */
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t white;
} color_t;

#define NO_WAVE ((uint8_t *)0)
#define NO_COLOR ((color_t *)0)       ///< Pointer constant to "no color"
#define NO_COLOR_OBJ ((Color *)0)     ///< Null color object
#define USEOLD_COLOR NO_COLOR         ///< Used old color agreement (if no color pointer)
#define USEOLD_COLOR_OBJ NO_COLOR_OBJ ///< Alternative use old color agreement

void ScaleColor(color_t * self, uint8_t k);
void MixColor(color_t * self, color_t c2);
void MixColorScaled(color_t * self, color_t c2, uint8_t k);
bool ColorIsEqual(color_t * self, color_t c2);
bool ColorNotEqual(color_t * c1, color_t c2);
void SetColor(color_t * self, color_t c2);
void SetColorByRgb(color_t * self, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void WriteByteStreamTo(color_t * self, uint8_t raw[sizeof(color_t)]);

extern const color_t *p_noColor;
extern const color_t cBlack;
extern const color_t cRed;
extern const color_t cGreen;
extern const color_t cBlue;
extern const color_t cMagenta;
extern const color_t cCyan;
extern const color_t cYellow;
extern const color_t cWhite;
extern const color_t cWarmWhite;
extern const color_t cColdWhite;
extern const color_t cSpookyWhite;

#ifdef __cplusplus
}
#endif