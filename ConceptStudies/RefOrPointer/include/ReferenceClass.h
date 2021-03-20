#pragma once
#include "utils.h"

class ColorR {
  public:
    Color_t _color; 
    
    ColorR(void) { SetColor(0, 0, 0); };
    ColorR(uint8_t r, uint8_t g, uint8_t b) { SetColor(r, g, b); };
    void SetColor(uint8_t r, uint8_t g, uint8_t b) {
        _color.red   = r;
        _color.green = g;
        _color.blue  = b;
    }
    void SetColor(const Color_t color) { memcpy(&_color, &color, sizeof(Color_t)); }

    Color_t GetColor(void) const { return _color; }

    ColorR operator=(ColorR const * c2) {
        SetColor(c2->_color);
        return *this;
    }

    ColorR & operator*(uint8_t k) {
        this->_color.red   = (uint8_t)((uint16_t)this->_color.red * k / 0xFF);
        this->_color.green = (uint8_t)((uint16_t)this->_color.green * k / 0xFF);
        this->_color.blue  = (uint8_t)((uint16_t)this->_color.blue * k / 0xFF);
        return *this;
    }

    ColorR & operator+(ColorR &c2) {
        this->_color.red   = this->_color.red + c2._color.red;
        this->_color.green = this->_color.green + c2._color.green;
        this->_color.blue  = this->_color.blue + c2._color.blue;
        return *this;
    }

    bool operator==(ColorR const &c2) {
    return ((_color.blue == c2._color.blue) && (_color.green == c2._color.green) && (_color.red == c2._color.red));
    }
    bool operator!=(ColorR const &c2) {
    return ((_color.blue != c2._color.blue) || (_color.green != c2._color.green) || (_color.red != c2._color.red));
    }
};


/* Alternative Approach */
// ColorR & operator*(ColorR &c1, uint8_t k) {
//     c1._color.red   = (uint8_t)((uint16_t)c1._color.red * k / 0xFF);
//     c1._color.green = (uint8_t)((uint16_t)c1._color.green * k / 0xFF);
//     c1._color.blue  = (uint8_t)((uint16_t)c1._color.blue * k / 0xFF);
//     return c1;
// }

// ColorR & operator+(ColorR &c1, ColorR &c2) {
//     c1._color.red   = c1._color.red + c2._color.red;
//     c1._color.green = c1._color.green + c2._color.green;
//     c1._color.blue  = c1._color.blue + c2._color.blue;
//     return c1;
// }