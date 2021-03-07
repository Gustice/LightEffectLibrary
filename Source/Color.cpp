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

/** @name Color structure definition
 *  @details Different predefined colors
 *  @note The colors are defined that the sum of all channels equals roughly 100 %,
 *    so that a Red should appear equally bright to white (where all channels are switched on,
 *    but with 33% at each channel).
 *    However the efficiency of each channel is not considered (maybe in derived classes or driver layer)
 */
///@{
/// Black color structure constant
const Color_t color_Black = {0x00, 0x00, 0x00, 0x00};
/// Red color structure constant
const Color_t color_Red = {0xFF, 0x00, 0x00, 0x00};
/// Green color structure constant
const Color_t color_Green = {0x00, 0xFF, 0x00, 0x00};
/// Blue color structure constant
const Color_t color_Blue = {0x00, 0x00, 0xFF, 0x00};
/// Magenta color structure constant
const Color_t color_Magenta = {0x80, 0x00, 0x80, 0x00};
/// Cyan color structure constant
const Color_t color_Cyan = {0x00, 0x80, 0x80, 0x00};
/// Yellow color structure constant
const Color_t color_Yellow = {0x80, 0x80, 0x00, 0x00};
/// White color structure constant
const Color_t color_White = {0x55, 0x55, 0x55, 0x00};
/// Warm white color structure constant (with higher red channel value)
const Color_t color_WarmWhite = {0x7F, 0x4F, 0x4F, 0x00};
/// Black color structure constant (with higher blue channel value)
const Color_t color_ColdWhite = {0x4F, 0x4F, 0x7F, 0x00};
/// Black color structure constant (with higher green channel value)
const Color_t color_SpookyWhite = {0x4F, 0x7F, 0x4F, 0x00};

/// Black color object constant
const Color CBlack(color_Black);
/// Red color object constant
const Color CRed(color_Red);
/// Green color object constant
const Color CGreen(color_Green);
/// Blue color object constant
const Color CBlue(color_Blue);
/// Magenta color object constant
const Color CMagenta(color_Magenta);
/// Cyan color object constant
const Color CCyan(color_Cyan);
/// Yellow color object constant
const Color CYellow(color_Yellow);
/// White color object constant
const Color CWhite(color_White);
/// Warm white color object constant (with higher red channel value)
const Color CWarmWhite(color_WarmWhite);
/// Black color object constant (with higher blue channel value)
const Color CColdWhite(color_ColdWhite);
/// Black color object constant (with higher green channel value)
const Color CSpookyWhite(color_SpookyWhite);
///@}

const Color_t *p_noColor    = (Color_t *)0;
const Color *  p_noColorObj = (Color *)0;

//@todo test this and differentiate to approach below
// Color Color::operator*(uint8_t k){
//     _color.red   = (uint8_t)((uint16_t)_color.red * k / 0xFF);
//     _color.green = (uint8_t)((uint16_t)_color.green * k / 0xFF);
//     _color.blue  = (uint8_t)((uint16_t)_color.blue * k / 0xFF);
//     _color.white = (uint8_t)((uint16_t)_color.white * k / 0xFF);
//     return _color;
// };

// Color Color::operator+(Color c2) {
//     _color.red   = _color.red + c2._color.red;
//     _color.green = _color.green + c2._color.green;
//     _color.blue  = _color.blue + c2._color.blue;
//     _color.white = _color.white + c2._color.white;
//     return _color;
// };

//@todo Change operator overwrite to reference Types
//@optimize Calling of calculation with inline calc

/**
 * @brief Scale color by scaling each color-channel by given factor
 *
 * @param c1 Color
 * @param k Scale-factor uint8_t.Max is fullscale
 * @return Scaled Color
 */
Color operator*(Color c1, uint8_t k) {
    c1._color.red   = (uint8_t)((uint16_t)c1._color.red * k / 0xFF);
    c1._color.green = (uint8_t)((uint16_t)c1._color.green * k / 0xFF);
    c1._color.blue  = (uint8_t)((uint16_t)c1._color.blue * k / 0xFF);
    c1._color.white = (uint8_t)((uint16_t)c1._color.white * k / 0xFF);
    return c1;
}

/**
 * @brief Overlaying to colors my adding each color channel value
 *
 * @note Colors channels should not overlap since there is no automatic ratio correction.
 *    Use *-Operator first in order to scale each color value a a way that no overflow can occur
 *
 * @param c1 Color 1
 * @param c2 Color 2
 * @return Overlayed Color
 */
Color operator+(Color c1, Color c2) {
    c1._color.red   = c1._color.red + c2._color.red;
    c1._color.green = c1._color.green + c2._color.green;
    c1._color.blue  = c1._color.blue + c2._color.blue;
    c1._color.white = c1._color.white + c2._color.white;
    return c1;
}
