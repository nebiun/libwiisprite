#include <wiisprite.h>

Color::Color()
{
	// Initialize to black
	r = 0;
	g = 0;
	b = 0;
}

Color::Color(u8 r, u8 g, u8 b)
{
	// Initialize to color
	this->r = r;
	this->g = g;
	this->b = b;
}

Color::Color(uint32_t color)
{
	// Set 3 individual values from 24bit color
	r = color & 0xFF;
	g = (color >> 8) & 0xFF;
	b = (color >> 16) & 0xFF;
}

Color::~Color()
{
	// Empty destructor
}

uint32_t Color::get24BitColor()
{
	// Grab 24bit color value from 3 individual values
	return b << 16 | g << 8 | r;
}

void Color::set24BitColor(uint32_t color)
{
	// Set 3 individual values from 24bit color
	r = color & 0xFF;
	g = (color >> 8) & 0xFF;
	b = (color >> 16) & 0xFF;
}

void Color::setColor(u8 r, u8 g, u8 b)
{
	// Initialize to color
	this->r = r;
	this->g = g;
	this->b = b;
}

GXColor Color::getGXColor()
{
	GXColor gCol;
	
	gCol.r = r;
	gCol.g = g;
	gCol.b = b;
	gCol.a = 255;
	
	return gCol;
}
