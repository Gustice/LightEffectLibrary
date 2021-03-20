/**
 * @file Color_t.h
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
} Color_t;

#define NO_WAVE ((uint8_t *)0)
#define NO_COLOR ((Color_t *)0)       ///< Pointer constant to "no color"
#define NO_COLOR_OBJ ((Color *)0)     ///< Null color object
#define USEOLD_COLOR NO_COLOR         ///< Used old color agreement (if no color pointer)
#define USEOLD_COLOR_OBJ NO_COLOR_OBJ ///< Alternative use old color agreement

void ScaleColor(Color_t * self, uint8_t k);
void MixColor(Color_t * self, Color_t c2);
void MixColorScaled(Color_t * self, Color_t c2, uint8_t k);
bool ColorIsEqual(Color_t * self, Color_t c2);
bool ColorNotEqual(Color_t * c1, Color_t c2);
void SetColor(Color_t * self, Color_t c2);
void SetColorByRgb(Color_t * self, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void WriteByteStreamTo(Color_t * self, uint8_t raw[sizeof(Color_t)]);

extern const Color_t *p_noColor;
extern const Color_t cBlack;
extern const Color_t cRed;
extern const Color_t cGreen;
extern const Color_t cBlue;
extern const Color_t cMagenta;
extern const Color_t cCyan;
extern const Color_t cYellow;
extern const Color_t cWhite;
extern const Color_t cWarmWhite;
extern const Color_t cColdWhite;
extern const Color_t cSpookyWhite;

#ifdef __cplusplus
}
#endif