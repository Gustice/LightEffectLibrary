/**
 * @file Color.h
 * @author Gustice
 * @brief   Color-Class and color Definitions
 * @details Defintion of Color Class with Predefined Colors as static ressource
 *
 * @version 0.1
 * @date 2019-09-30
 *
 * @copyright Copyright (c) 2019
 */
#pragma once

#include <stdint.h>

/**
 * @brief Color Structure 
 * @details Defines red/green/blue and white color channel values
 */
typedef struct Color_def {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t white;
} Color_t;

#define NO_COLOR ((Color_t *)0)       ///< Pointer constant to "no color"
#define NO_COLOR_OBJ ((Color *)0)     ///< Null color object
#define USEOLD_COLOR NO_COLOR         ///< Used old color agreement (if no color pointer)
#define USEOLD_COLOR_OBJ NO_COLOR_OBJ ///< Alternative use old color agreement

/**
 * @brief   Color class
 *
 * @details Defines color data and common operations with color objects
 *  \li Supports RGB with overlapping white channel (e.g. SK6812-LED)
 *  \li Supprts Operator overloading with '+' and '*'
 *  \li Return formatted data stream that can be directly sent to an LED
 */
class Color {
    friend Color operator+(Color c1, Color c2);
    friend Color operator*(Color c1, uint8_t k);

  public:
    Color(void);
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0);
    Color(Color_t color);

    void SetColor(Color_t color);
    void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0);

    Color_t GetColor(void);
    void WriteByteStreamTo(uint8_t raw[sizeof(Color_t)]);

  private:
    Color_t _color; ///< Color data
};


Color operator+(Color c1, Color c2);
Color operator*(Color c1, uint8_t k);

/** @name Color structure definition
 *  @details Different predefined colors
 *  @note The colors are defined that the sum of all channels equals roughly 100 %,
 *    so that a Red should appear equally bright to white (where all channels are switched on,
 *    but with 33% at each channel).
 *    However the efficiency of each channel is not considered (maybe in derived classes or driver layer)
 */
///@{
extern const Color_t color_Black;       ///< Black color structure constant
extern const Color_t color_Red;         ///< Red color structure constant
extern const Color_t color_Green;       ///< Green color structure constant
extern const Color_t color_Blue;        ///< Blue color structure constant
extern const Color_t color_Magenta;     ///< Magenta color structure constant
extern const Color_t color_Cyan;        ///< Cyan color structure constant
extern const Color_t color_Yellow;      ///< Yellow color structure constant
extern const Color_t color_White;       ///< White color structure constant
extern const Color_t color_WarmWhite;   ///< Warm white color structure constant (with higher red channel value)
extern const Color_t color_ColdWhite;   ///< Black color structure constant (with higher blue channel value)
extern const Color_t color_SpookyWhite; ///< Black color structure constant (with higher green channel value)

extern const Color _ctBlack;       ///< Black color object constant
extern const Color _ctRed;         ///< Red color object constant
extern const Color _ctGreen;       ///< Green color object constant
extern const Color _ctBlue;        ///< Blue color object constant
extern const Color _ctMagenta;     ///< Magenta color object constant
extern const Color _ctCyan;        ///< Cyan color object constant
extern const Color _ctYellow;      ///< Yellow color object constant
extern const Color _ctWhite;       ///< White color object constant
extern const Color _ctWarmWhite;   ///< Warm white color object constant (with higher red channel value)
extern const Color _ctColdWhite;   ///< Black color object constant (with higher blue channel value)
extern const Color _ctSpookyWhite; ///< Black color object constant (with higher green channel value)
///@}
