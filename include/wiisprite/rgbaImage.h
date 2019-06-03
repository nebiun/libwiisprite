#pragma once

#include <gctypes.h>
#include "ftText.h"

using namespace wsp;

class rgbaImage : public Image {
protected:
	rgbaImage(const unsigned int width, const unsigned int height);

public:
	virtual ~rgbaImage(void);

	class pixel {
	public:
		u8 r,g,b,a;
		void operator~(void);
		bool operator!=(const pixel&);
		bool operator==(const pixel&);

		pixel(void);
		pixel(const u8 r, const u8 g, const u8 b);
		pixel(const u8 r, const u8 g, const u8 b, const u8 a);
	};

protected:
	rgbaImage::pixel &operator() (const unsigned int x, const unsigned int y);
	rgbaImage::pixel &getpixel(const unsigned int x, const unsigned int y);
	pixel *_RGBApixels;
	TextRender font;
	void _Flush();

private:
	void _convertPixels();

	static const unsigned int tilesize=4;
};
