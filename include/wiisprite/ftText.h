#ifndef __TEXTRENDER_H
#define __TEXTRENDER_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_X 0
#define DEFAULT_Y 0

#define DEFAULT_Y_CUSHION 2
#define DEFAULT_TAB_SPACE 50

#include <stdint.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "ftColor.h"

typedef struct
{
	int left;
	int top;
	int width;
	int rows;
	int advance;
	uint8_t *data;
} BUFFER;

class TextRender
{
	public:
		TextRender();
		~TextRender();
		
		void setFont(const u8 *fontbuf, const u32 fontsize);
		void setColor(Color c);
		void setColor(u8 r, u8 g, u8 b);
		void setSize(int s);
		void reset();
		
		void setBuffer(uint8_t *buf, int width, int height);
		void renderSimple(const char *out);
		void render(const char *fmt, ...);
	private:
		void Blit(uint8_t *bmpBuf, int runWidth, int runHeight, int left, int top);
		
		static FT_Library library;
		static int renderCount;
		
		FT_Face face;
		
		int _x;
		int _y;

		uint8_t *_buf;
		int _width;
		int _height;
		int _fontheight;
		Color _color;
		
		BUFFER spriteBuf[256];
};

#ifdef __cplusplus
}
#endif

#endif


