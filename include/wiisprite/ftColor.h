#ifndef __COLOR_H
#define __COLOR_H

#ifdef __cplusplus
extern "C" {
#endif

#define FULL_VISIBLE 255

#include <stdint.h>
#include <wiisprite.h>

class Color
{
	public:
		u8 r;
		u8 g;
		u8 b;
		
		Color();
		Color(uint32_t color);
		Color(u8 r, u8 g, u8 b);
		~Color();
		
		uint32_t get24BitColor();
		void set24BitColor(uint32_t color);
		void setColor(u8 r, u8 g, u8 b);
		GXColor getGXColor();
};

#ifdef __cplusplus
}
#endif

#endif


