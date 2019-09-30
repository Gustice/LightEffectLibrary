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
#include <string.h>

const Color_t color_Black       = {0x00, 0x00, 0x00, 0x00};
const Color_t color_Red         = {0xFF, 0x00, 0x00, 0x00};
const Color_t color_Green       = {0x00, 0xFF, 0x00, 0x00};
const Color_t color_Blue        = {0x00, 0x00, 0xFF, 0x00};
const Color_t color_Magenta     = {0x80, 0x00, 0x80, 0x00};
const Color_t color_Cyan        = {0x00, 0x80, 0x80, 0x00};
const Color_t color_Yellow      = {0x80, 0x80, 0x00, 0x00};
const Color_t color_White       = {0x55, 0x55, 0x55, 0x00};
const Color_t color_WarmWhite   = {0x7F, 0x4F, 0x4F, 0x00};
const Color_t color_ColdWhite   = {0x4F, 0x4F, 0x7F, 0x00};
const Color_t color_SpookyWhite = {0x4F, 0x7F, 0x4F, 0x00};

const Color _ctBlack(color_Black);
const Color _ctRed(color_Red);
const Color _ctGreen(color_Green);
const Color _ctBlue(color_Blue);
const Color _ctMagenta(color_Magenta);
const Color _ctCyan(color_Cyan);
const Color _ctYellow(color_Yellow);
const Color _ctWhite(color_White);
const Color _ctWarmWhite(color_WarmWhite);
const Color _ctColdWhite(color_ColdWhite);
const Color _ctSpookyWhite(color_SpookyWhite);

Color::Color(void) { SetColor(0, 0, 0, 0); }

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) { SetColor(r, g, b, w); }

Color::Color(Color_t color) { memcpy(&_color, &color, sizeof(Color_t)); }

void Color::SetColor(Color_t color) { memcpy(&_color, &color, sizeof(Color_t)); }

void Color::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    _color.red   = r;
    _color.green = g;
    _color.blue  = b;
    _color.white = w;
}

Color_t Color::GetColor(void) { return _color; }

void Color::WriteByteStreamTo(uint8_t raw[sizeof(Color_t)]) {
    raw[0] = _color.blue;
    raw[1] = _color.green;
    raw[2] = _color.red;
    raw[3] = _color.white;
}

Color operator*(Color c1, uint8_t k) {
    c1._color.red   = (uint8_t)((uint16_t)c1._color.red * k / 0xFF);
    c1._color.green = (uint8_t)((uint16_t)c1._color.green * k / 0xFF);
    c1._color.blue  = (uint8_t)((uint16_t)c1._color.blue * k / 0xFF);
    c1._color.white = (uint8_t)((uint16_t)c1._color.white * k / 0xFF);
    return c1;
}

Color operator+(Color c1, Color c2) {
    c1._color.red   = c1._color.red + c2._color.red;
    c1._color.green = c1._color.green + c2._color.green;
    c1._color.blue  = c1._color.blue + c2._color.blue;
    c1._color.white = c1._color.white + c2._color.white;
    return c1;
}
