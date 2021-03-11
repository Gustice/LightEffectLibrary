/**
 * @file Color.cpp
 * @author Gustice
 * @brief Implementation of Color-Class Color.h
 * @version 0.1
 * @date 2019-09-30
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "Color.h"
#include <stdbool.h>
#include <string.h>

/** @name Color structure definition
 *  @details Different predefined colors
 *  @note The colors are defined that the sum of all channels equals roughly 100 %,
 *    so that a Red should appear equally bright to white (where all channels are switched on,
 *    but with 33% at each channel).
 *    However the efficiency of each channel is not considered (maybe in derived classes or driver layer)
 */
///@{
/// Black color structure constant
const color_t color_Black = {0x00, 0x00, 0x00, 0x00};
/// Red color structure constant
const color_t color_Red = {0xFF, 0x00, 0x00, 0x00};
/// Green color structure constant
const color_t color_Green = {0x00, 0xFF, 0x00, 0x00};
/// Blue color structure constant
const color_t color_Blue = {0x00, 0x00, 0xFF, 0x00};
/// Magenta color structure constant
const color_t color_Magenta = {0x80, 0x00, 0x80, 0x00};
/// Cyan color structure constant
const color_t color_Cyan = {0x00, 0x80, 0x80, 0x00};
/// Yellow color structure constant
const color_t color_Yellow = {0x80, 0x80, 0x00, 0x00};
/// White color structure constant
const color_t color_White = {0x55, 0x55, 0x55, 0x00};
/// Warm white color structure constant (with higher red channel value)
const color_t color_WarmWhite = {0x7F, 0x4F, 0x4F, 0x00};
/// Black color structure constant (with higher blue channel value)
const color_t color_ColdWhite = {0x4F, 0x4F, 0x7F, 0x00};
/// Black color structure constant (with higher green channel value)
const color_t color_SpookyWhite = {0x4F, 0x7F, 0x4F, 0x00};

///@}

const color_t *p_noColor = (color_t *)0;

void ScaleColor(color_t *self, uint8_t k) {
    self->red   = (uint8_t)((uint16_t)self->red * k / 0xFF);
    self->green = (uint8_t)((uint16_t)self->green * k / 0xFF);
    self->blue  = (uint8_t)((uint16_t)self->blue * k / 0xFF);
    self->white = (uint8_t)((uint16_t)self->white * k / 0xFF);
}

void MixColor(color_t *self, color_t c2) {
    self->red   = self->red + c2.red;
    self->green = self->green + c2.green;
    self->blue  = self->blue + c2.blue;
    self->white = self->white + c2.white;
}

bool ColorIsEqual(color_t *self, color_t c2) {
    return ((self->blue == c2.blue) && (self->green == c2.green) && (self->red == c2.red) && (self->white == c2.white));
}

bool ColorNotEqual(color_t *self, color_t c2) {
    return ((self->blue != c2.blue) || (self->green != c2.green) || (self->red != c2.red) || (self->white != c2.white));
}

void SetColor(color_t *self, color_t c2) {
    self->red   = c2.red;
    self->green = c2.green;
    self->blue  = c2.blue;
    self->white = c2.white;
}

void SetColor(color_t *self, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    self->red   = r;
    self->green = g;
    self->blue  = b;
    self->white = w;
}

void WriteByteStreamTo(color_t *self, uint8_t raw[sizeof(color_t)]) {
    raw[0] = self->blue;
    raw[1] = self->green;
    raw[2] = self->red;
    raw[3] = self->white;
}
