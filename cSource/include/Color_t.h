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

#include <stdint.h>
#include <string.h>



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
#define NO_COLOR ((Color_t *)0)       ///< Pointer constant to "no color"
#define NO_COLOR_OBJ ((Color *)0)     ///< Null color object
#define USEOLD_COLOR NO_COLOR         ///< Used old color agreement (if no color pointer)
#define USEOLD_COLOR_OBJ NO_COLOR_OBJ ///< Alternative use old color agreement


void ScaleColor(color_t * self, uint8_t k);
void MixColor(color_t * self, color_t c2);
bool ColorIsEqual(color_t * self, color_t c2);
bool ColorNotEqual(color_t * c1, color_t c2);
void SetColor(color_t * self, color_t c2);
void SetColor(color_t * self, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void WriteByteStreamTo(color_t * self, uint8_t raw[sizeof(color_t)]);


extern const color_t *p_noColor;
extern const color_t color_Black;
extern const color_t color_Red;
extern const color_t color_Green;
extern const color_t color_Blue;
extern const color_t color_Magenta;
extern const color_t color_Cyan;
extern const color_t color_Yellow;
extern const color_t color_White;
extern const color_t color_WarmWhite;
extern const color_t color_ColdWhite;
extern const color_t color_SpookyWhite;

