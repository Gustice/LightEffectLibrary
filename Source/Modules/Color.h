

#pragma once

#include <stdint.h>

typedef struct Color_def {
	uint8_t alpha;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t white;
}Color_type;

#define NO_COLOR ((Color_type *)0) 		///< Pointer constant to "no color"
#define NO_COLOR_OBJ ((Color *)0)		///< Null color object
#define USEOLD_COLOR NO_COLOR			///< Used old color agreement (if no color pointer)
#define USEOLD_COLOR_OBJ NO_COLOR_OBJ 	///< Alternative use old color agreement

/***********************************************************************************************//**
 * @brief 	Color class 
 * 
 * @details Defines color data and common operations with the colors
 * 
 **************************************************************************************************/
class Color
{
	friend Color operator + (Color c1, Color c2);
	friend Color operator * (Color c1, uint8_t k);
	public:
	Color(void);
	Color(uint8_t a, uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0);
	Color(uint8_t r, uint8_t g, uint8_t b);

	void SetColor(Color_type color);
	void SetColor(uint8_t a, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
	
	Color_type GetColor(void);
	void GetByteStream(uint8_t color[5]);
	uint8_t getR(void);
	uint8_t getG(void);
	uint8_t getB(void);
	
	private:
	Color_type _color;
};

Color operator + (Color c1, Color c2);
Color operator * (Color c1, uint8_t k);



extern const Color_type color_Black;
extern const Color_type color_Red;
extern const Color_type color_Green;
extern const Color_type color_Blue;
extern const Color_type color_Magenta;
extern const Color_type color_Cyan;
extern const Color_type color_Yellow;
extern const Color_type color_White;
extern const Color_type color_WarmWhite;
extern const Color_type color_ColdWhite;
extern const Color_type color_SpookieWhite;

extern const Color _ctBlack;
extern const Color _ctRed;
extern const Color _ctGreen;
extern const Color _ctBlue;
extern const Color _ctMagenta;
extern const Color _ctCyan;
extern const Color _ctYellow;
extern const Color _ctWhite;
extern const Color _ctWarmWhite;
extern const Color _ctColdWhite;
extern const Color _ctSpookieWhite;