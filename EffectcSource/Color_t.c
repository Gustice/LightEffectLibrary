/**
 * @file Color_t.c
 * @author Gustice
 * @brief Implementation of Color-Class Color_t.h
 * @version 0.1
 * @date 2019-09-30
 *
 * @copyright Copyright (c) 2019
 */

#include "color_t.h"
#include <stdbool.h>
#include <string.h>

const Color_t cBlack = {0x00, 0x00, 0x00, 0x00};
const Color_t cRed = {0xFF, 0x00, 0x00, 0x00};
const Color_t cGreen = {0x00, 0xFF, 0x00, 0x00};
const Color_t cBlue = {0x00, 0x00, 0xFF, 0x00};
const Color_t cMagenta = {0x80, 0x00, 0x80, 0x00};
const Color_t cCyan = {0x00, 0x80, 0x80, 0x00};
const Color_t cYellow = {0x80, 0x80, 0x00, 0x00};
const Color_t cWhite = {0x55, 0x55, 0x55, 0x00};
const Color_t cWarmWhite = {0x7F, 0x4F, 0x4F, 0x00};
const Color_t cColdWhite = {0x4F, 0x4F, 0x7F, 0x00};
const Color_t cSpookyWhite = {0x4F, 0x7F, 0x4F, 0x00};

const Color_t * pNoColor = (Color_t *)0;

void ScaleColor(Color_t *self, uint8_t k) {
    self->red   = (uint8_t)((uint16_t)self->red * k / 0xFF);
    self->green = (uint8_t)((uint16_t)self->green * k / 0xFF);
    self->blue  = (uint8_t)((uint16_t)self->blue * k / 0xFF);
    self->white = (uint8_t)((uint16_t)self->white * k / 0xFF);
}

void MixColor(Color_t *self, Color_t c2) {
    self->red   = self->red + c2.red;
    self->green = self->green + c2.green;
    self->blue  = self->blue + c2.blue;
    self->white = self->white + c2.white;
}

void MixColorScaled(Color_t * self, Color_t c2, uint8_t k)
{
    ScaleColor(&c2, k);
    ScaleColor(self, (UINT8_MAX - k));
    MixColor(self, c2);
}

bool ColorIsEqual(Color_t *self, Color_t c2) {
    return ((self->blue == c2.blue) && (self->green == c2.green) && (self->red == c2.red) && (self->white == c2.white));
}

bool ColorNotEqual(Color_t *self, Color_t c2) {
    return ((self->blue != c2.blue) || (self->green != c2.green) || (self->red != c2.red) || (self->white != c2.white));
}

void SetColor(Color_t *self, Color_t c2) {
    self->red   = c2.red;
    self->green = c2.green;
    self->blue  = c2.blue;
    self->white = c2.white;
}

void SetColorByRgb(Color_t *self, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    self->red   = r;
    self->green = g;
    self->blue  = b;
    self->white = w;
}

#ifndef USE_OWN_STREAM_CONVERTER
void WriteByteStreamTo(Color_t *self, uint8_t raw[sizeof(Color_t)]) {
    raw[0] = self->blue;
    raw[1] = self->green;
    raw[2] = self->red;
    raw[3] = self->white;
}
#endif