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
#define NO_COLOR ((color_t *)0)       ///< Pointer constant to "no color"
#define NO_COLOR_OBJ ((Color *)0)     ///< Null color object
#define USEOLD_COLOR NO_COLOR         ///< Used old color agreement (if no color pointer)
#define USEOLD_COLOR_OBJ NO_COLOR_OBJ ///< Alternative use old color agreement

/**
 * @brief   Color class
 *
 * @details Defines color data and common operations with color objects
 *  \li Supports RGB with overlapping white channel (e.g. SK6812-LED)
 *  \li Supprts Operator overloading with '+' and '*' (Combined operation with MixWith)
 *  \li Return formatted data stream that can be directly sent to an LED
 */
class Color {
    friend Color operator+(Color c1, Color c2);
    friend Color operator*(Color c1, uint8_t k);

  private:
    color_t _color; ///< Color data

  public:
    /**
     * @brief   Construct a new Color object
     * @details Default color will be Black \ref _ctBlack
     */
    Color(void) { SetColor(0, 0, 0, 0); };

    /**
     * @brief   Construct a new Color object according to rgb color-channel-values
     * @details This constructor also takes an overlaping white channel
     * @param r Red channel
     * @param g Green channel
     * @param b Blue channel
     * @param w White channel (zero by default if not assigned)
     */
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0) { SetColor(r, g, b, w); };

    /**
     * @brief Construct a new Color object according to color_t structure
     * @param color Color to set
     */
    Color(color_t color) { memcpy(&_color, &color, sizeof(color_t)); };
    /**
     * @brief Construct a new Color object according to color_t structure
     * @param color Color to set
     */
    Color(color_t *color) { memcpy(&_color, color, sizeof(color_t)); };

    /**
     * @brief Set the Color object according to color_t Array
     * @param color Color to set
     */
    void SetColor(const color_t *color) { memcpy(&_color, color, sizeof(color_t)); }
    /**
     * @brief Set the Color object according to color_t Array
     * @param color Color to set
     */
    void SetColor(const color_t color) { memcpy(&_color, &color, sizeof(color_t)); }

    /**
     * @brief Set the Color object according to rgb color-channel values
     * @param r Red channel
     * @param g Green channel
     * @param b Blue channel
     * @param w White channel (zero by default if not assigned)
     */
    void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0) {
        _color.red   = r;
        _color.green = g;
        _color.blue  = b;
        _color.white = w;
    }

    /**
     * @brief Returns Color as color_t structure
     * @return color_t Color
     */
    color_t GetColor(void) const { return _color; }

    /**
     * @brief Scaling and overlaying with given color.
     * @param c2 Color 2
     * @param k scaling factor
     * @return Overlayed Color
     */
    Color MixWith(const Color c2, uint8_t k = 0x7F) {
        *this = (*this * (UINT8_MAX - k)) + (c2 * k);
        return *this;
    }

    /**
     * @brief Writes Color as formatted bytestream to location
     * @param color
     */
    void WriteByteStreamTo(uint8_t raw[sizeof(color_t)]) {
        raw[0] = _color.blue;
        raw[1] = _color.green;
        raw[2] = _color.red;
        raw[3] = _color.white;
    }

    // Color operator*(uint8_t k);
    // Color operator+(Color * c2);
    Color operator=(Color const *c2) {
        SetColor(c2->_color);
        return *this;
    }

    bool operator==(Color const &c2) {
        return ((_color.blue == c2._color.blue) && (_color.green == c2._color.green) && (_color.red == c2._color.red) &&
                (_color.white == c2._color.white));
    }
    bool operator!=(Color const &c2) {
        return ((_color.blue != c2._color.blue) || (_color.green != c2._color.green) || (_color.red != c2._color.red) ||
                (_color.white != c2._color.white));
    }
};

Color operator*(Color c1, uint8_t k);
Color operator+(Color c1, Color c2);

extern const color_t *pNoColor;
extern const Color *  pNoColorObj;

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

// @todo this might be useless
extern const Color CBlack;
extern const Color CRed;
extern const Color CGreen;
extern const Color CBlue;
extern const Color CMagenta;
extern const Color CCyan;
extern const Color CYellow;
extern const Color CWhite;
extern const Color CWarmWhite;
extern const Color CColdWhite;
extern const Color CSpookyWhite;
