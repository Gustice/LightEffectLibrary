/*
 * _color.cpp
 *
 * Created: 04.01.2019 22:53:27
 *  Author: Jakob
 */ 

#include "Color.h"
#include <string.h>



const Color_type color_Black =			{0xFF, 0x00, 0x00, 0x00, 0x00};
const Color_type color_Red =			{0xFF, 0xFF, 0x00, 0x00, 0x00};
const Color_type color_Green =			{0xFF, 0x00, 0xFF, 0x00, 0x00};
const Color_type color_Blue =			{0xFF, 0x00, 0x00, 0xFF, 0x00};
const Color_type color_Magenta =		{0xFF, 0x80, 0x00, 0x80, 0x00};
const Color_type color_Cyan =			{0xFF, 0x00, 0x80, 0x80, 0x00};
const Color_type color_Yellow =			{0xFF, 0x80, 0x80, 0x00, 0x00};
const Color_type color_White =			{0xFF, 0x55, 0x55, 0x55, 0x00};
const Color_type color_WarmWhite =		{0xFF, 0x7F, 0x4F, 0x4F, 0x00};
const Color_type color_ColdWhite =		{0xFF, 0x4F, 0x4F, 0x7F, 0x00};
const Color_type color_SpookieWhite =	{0xFF, 0x4F, 0x7F, 0x4F, 0x00};

const Color _ctBlack(0xFF, 0x00, 0x00, 0x00, 0x00);
const Color _ctRed(0xFF, 0xFF, 0x00, 0x00, 0x00);
const Color _ctGreen(0xFF, 0x00, 0xFF, 0x00, 0x00);
const Color _ctBlue(0xFF, 0x00, 0x00, 0xFF, 0x00);
const Color _ctMagenta(0xFF, 0x80, 0x00, 0x80, 0x00);
const Color _ctCyan(0xFF, 0x00, 0x80, 0x80, 0x00);
const Color _ctYellow(0xFF, 0x80, 0x80, 0x00, 0x00);
const Color _ctWhite(0xFF, 0x55, 0x55, 0x55, 0x00);
const Color _ctWarmWhite(0xFF, 0x7F, 0x4F, 0x4F, 0x00);
const Color _ctColdWhite(0xFF, 0x4F, 0x4F, 0x7F, 0x00);
const Color _ctSpookieWhite(0xFF, 0x4F, 0x7F, 0x4F, 0x00);


Color::Color(void) {
	SetColor(0,0,0,0,0);
}

Color::Color(uint8_t a, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
	SetColor(a,r,g,b,w);
}

Color::Color(uint8_t r, uint8_t g, uint8_t b) {
	SetColor(0xFF,r,g,b,0);
}

void Color::SetColor(Color_type color) {
	memcpy(&_color, &color, sizeof(Color_type));
}


void Color::SetColor(uint8_t a, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
	_color.alpha = a;
	_color.red = r;
	_color.green = g;
	_color.blue = b;
	_color.white = w;
}


void Color::GetByteStream(uint8_t clr[5]) {
	clr[0] = _color.alpha;
	clr[1] = _color.blue;
	clr[2] = _color.green;
	clr[3] = _color.red;
	clr[4] = _color.white;
}


Color_type Color::GetColor(void) { 
	return _color; 
}

uint8_t Color::getR(void)
{
	return _color.red;
}

uint8_t Color::getG(void)
{
	return _color.green;
}

uint8_t Color::getB(void)
{
	return _color.blue;
}

Color operator * (Color c1, uint8_t k) {
	c1._color.red = (uint8_t)((uint16_t)c1._color.red * k / 0xFF);
	c1._color.green = (uint8_t)((uint16_t)c1._color.green * k / 0xFF);
	c1._color.blue = (uint8_t)((uint16_t)c1._color.blue * k / 0xFF);
	c1._color.white = (uint8_t)((uint16_t)c1._color.white * k / 0xFF);
	return c1;
}

Color operator + (Color c1, Color c2) {
	c1._color.red = c1._color.red + c2._color.red;
	c1._color.green = c1._color.green + c2._color.green;
	c1._color.blue = c1._color.blue + c2._color.blue;
	c1._color.white = c1._color.white + c2._color.white;
	return c1;
}

