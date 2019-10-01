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
const Color_t color_Black       = {0x00, 0x00, 0x00, 0x00}; ///< Black color structure constant
const Color_t color_Red         = {0xFF, 0x00, 0x00, 0x00}; ///< Red color structure constant
const Color_t color_Green       = {0x00, 0xFF, 0x00, 0x00}; ///< Green color structure constant
const Color_t color_Blue        = {0x00, 0x00, 0xFF, 0x00}; ///< Blue color structure constant
const Color_t color_Magenta     = {0x80, 0x00, 0x80, 0x00}; ///< Magenta color structure constant
const Color_t color_Cyan        = {0x00, 0x80, 0x80, 0x00}; ///< Cyan color structure constant
const Color_t color_Yellow      = {0x80, 0x80, 0x00, 0x00}; ///< Yellow color structure constant
const Color_t color_White       = {0x55, 0x55, 0x55, 0x00}; ///< White color structure constant
const Color_t color_WarmWhite   = {0x7F, 0x4F, 0x4F, 0x00}; ///< Warm white color structure constant (with higher red channel value)
const Color_t color_ColdWhite   = {0x4F, 0x4F, 0x7F, 0x00}; ///< Black color structure constant (with higher blue channel value)
const Color_t color_SpookyWhite = {0x4F, 0x7F, 0x4F, 0x00}; ///< Black color structure constant (with higher green channel value)

const Color _ctBlack(color_Black);             ///< Black color object constant
const Color _ctRed(color_Red);                 ///< Red color object constant
const Color _ctGreen(color_Green);             ///< Green color object constant
const Color _ctBlue(color_Blue);               ///< Blue color object constant
const Color _ctMagenta(color_Magenta);         ///< Magenta color object constant
const Color _ctCyan(color_Cyan);               ///< Cyan color object constant
const Color _ctYellow(color_Yellow);           ///< Yellow color object constant
const Color _ctWhite(color_White);             ///< White color object constant
const Color _ctWarmWhite(color_WarmWhite);     ///< Warm white color object constant (with higher red channel value)
const Color _ctColdWhite(color_ColdWhite);     ///< Black color object constant (with higher blue channel value)
const Color _ctSpookyWhite(color_SpookyWhite); ///< Black color object constant (with higher green channel value)
///@}

/**
 * @brief   Construct a new Color object
 * @details Default color will be Black \ref _ctBlack
 */
Color::Color(void) { SetColor(0, 0, 0, 0); }

/**
 * @brief   Construct a new Color object according to rgb color-channel-values
 * @details This constructor also takes an overlaping white channel
 * @param r Red channel
 * @param g Green channel
 * @param b Blue channel
 * @param w White channel (zero by default if not assigned)
 */
Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) { SetColor(r, g, b, w); }

/**
 * @brief Construct a new Color object according to Color_t Array
 * @param color
 */
Color::Color(Color_t color) { memcpy(&_color, &color, sizeof(Color_t)); }

/**
 * @brief Set the Color object according to Color_t Array
 *
 * @param color
 */
void Color::SetColor(Color_t color) { memcpy(&_color, &color, sizeof(Color_t)); }

/**
 * @brief Set the Color object according to rgb color-channel values
 *
 * @param r Red channel
 * @param g Green channel
 * @param b Blue channel
 * @param w White channel (zero by default if not assigned)
 */
void Color::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    _color.red   = r;
    _color.green = g;
    _color.blue  = b;
    _color.white = w;
}

/**
 * @brief Returns Color as Color_t structure
 *
 * @return Color_t Color
 */
Color_t Color::GetColor(void) { return _color; }

/**
 * @brief Writes Color as formatted bytestream to location
 *
 * @param color
 */
void Color::WriteByteStreamTo(uint8_t raw[sizeof(Color_t)]) {
    raw[0] = _color.blue;
    raw[1] = _color.green;
    raw[2] = _color.red;
    raw[3] = _color.white;
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
Color operator*(Color c1, uint8_t k) {
    c1._color.red   = (uint8_t)((uint16_t)c1._color.red * k / 0xFF);
    c1._color.green = (uint8_t)((uint16_t)c1._color.green * k / 0xFF);
    c1._color.blue  = (uint8_t)((uint16_t)c1._color.blue * k / 0xFF);
    c1._color.white = (uint8_t)((uint16_t)c1._color.white * k / 0xFF);
    return c1;
}

/**
 * @brief Scale color by scaling each color-channel by given scale-factor
 *
 * @param c1 Color
 * @param k Scale-factor uint8_t.Max is fullscale
 * @return Scaled Color
 */
Color operator+(Color c1, Color c2) {
    c1._color.red   = c1._color.red + c2._color.red;
    c1._color.green = c1._color.green + c2._color.green;
    c1._color.blue  = c1._color.blue + c2._color.blue;
    c1._color.white = c1._color.white + c2._color.white;
    return c1;
}
