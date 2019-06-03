#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>

#include <wiisprite.h>

FT_Library TextRender::library;
int TextRender::renderCount = 0;

TextRender::TextRender()
{
	if(!renderCount)
	{
		// Initialize the library
		FT_Error error = FT_Init_FreeType(&library);
		
		if(error)
		{
			// Better handling in the future
			exit(0);
		}
	}
	
	// Keep track of instances
	renderCount++;
	
	// Init the sprite buffer
	memset(spriteBuf, 0, sizeof(BUFFER) * 256);

	// init cursor position
	_x = 0;
	_y = 0;
}

TextRender::~TextRender()
{
	// Keep track of instances
	renderCount--;
	
	if(!renderCount)
	{
		// Free the library (and all resources)
		FT_Done_FreeType(library);
	}
	
	// Free any sprites
	for(int i = 0; i < 256; i++)
	{
		if(spriteBuf[i].data) { free(spriteBuf[i].data); }
	}
}

void TextRender::setFont(const u8 *fontbuf, const u32 fontsize)
{
	// Initialize a font
	FT_Error error = FT_New_Memory_Face(library, fontbuf, fontsize, 0, &face);

	if ( error != 0 )
	{
			// Better handling in the future
			exit(0);
	}
	
	// Free any sprites
	for(int i = 0; i < 256; i++)
	{
		if(spriteBuf[i].data) { free(spriteBuf[i].data); }
	}
	
	// Init the sprite buffer
	memset(spriteBuf, 0, sizeof(BUFFER) * 256);
}

void TextRender::setColor(Color c)
{
	_color = c;
}

void TextRender::setColor(u8 r, u8 g, u8 b)
{
	this->_color.r = r;
	this->_color.g = g;
	this->_color.b = b;
}
void TextRender::setSize(int s)
{
	// Set up the font face (see freetype samples for magic values)
	FT_Set_Pixel_Sizes(face, 0, s);
	
	_fontheight = s;
}

void TextRender::setBuffer(uint8_t *buf, int width, int height)
{
	// Set up a buffer to render to (RGBA quads, most likely to be blitted into a DrawableImage and rendered using libwiisprite)
	_buf = buf;
	_width = width;
	_height = height;
}

void TextRender::Blit(uint8_t *bmpBuf, int runWidth, int runHeight, int left, int top)
{
	// Remember original widths
	int oWidth = runWidth;
	
	// Precalculate the width
	if((left + runWidth) >= _width)
	{
		// Run height needs adjustment
		runWidth = _width - left;
	}
	
	// Precalculate the height
	if((top + runHeight) >= _height)
	{
		// Run height needs adjustment
		runHeight = _height - top;
	}
	
	// Precalculate color
	uint32_t c = _color.r << 24 | _color.g << 16 | _color.b << 8;
	
	// Copy alpha data over, setting the color to the predefined color and the alpha to the value of the glyph
	for(int y = 0; y < runHeight; y++)
	{
		// Precalculate
		int sywidth = (y * oWidth);
		int dywidth = ((y + top) * _width);
		
		for(int x = 0; x < runWidth; x++)
		{
			// Precalculate
			int srcloc = x + sywidth;
			int dstloc = ((x + left) + dywidth) << 2;
			
			// Copy in alpha
			c &= 0xFFFFFF00;
			c |= bmpBuf[srcloc];
			
			// Copy data over
			memcpy(_buf + dstloc, &c, 4);
		}
	}
}

void TextRender::reset()
{
	_x = 0;
	_y = 0;
}

void TextRender::renderSimple(const char *out)
{
	// Shortcut from examples
	FT_GlyphSlot slot = face->glyph;
	
	// Render
	for(uint32_t i = 0; i < strlen(out); i++)
	{
		int charOut = out[i];
		
		if(charOut == '\r' || charOut == '\n')
		{
			// Newline
			_x = DEFAULT_X;
			_y += _fontheight + DEFAULT_Y_CUSHION;
			
			continue;
		}
		else if(charOut == '\t')
		{
			// Truncate to floor (nearest bounds)
			_x /= DEFAULT_TAB_SPACE;
			_x *= DEFAULT_TAB_SPACE;
			
			// Add tab stop
			_x += DEFAULT_TAB_SPACE;
			
			continue;
		}
		
		// Check if it's cached
		if(spriteBuf[charOut].data)
		{
			// It is!  Blit glyph to surface
			Blit(spriteBuf[charOut].data, spriteBuf[charOut].width, spriteBuf[charOut].rows, _x + spriteBuf[charOut].left, (_y + _fontheight) - spriteBuf[charOut].top);
			
			// Advance
			_x += spriteBuf[charOut].advance;
		}
		else
		{
			// Render glyph
			FT_Error error = FT_Load_Char(face, charOut, FT_LOAD_RENDER);
			if(error) continue;  /* ignore errors */
			
			// Blit glyph to surface
			Blit(slot->bitmap.buffer, slot->bitmap.width, slot->bitmap.rows, _x + slot->bitmap_left, (_y + _fontheight) - slot->bitmap_top);
			
			// Advance the position
			_x += slot->advance.x >> 6;
			
			// Create buffer
			spriteBuf[charOut].data = (uint8_t *)malloc(slot->bitmap.width * slot->bitmap.rows);
			
			// Set aux data
			spriteBuf[charOut].width = slot->bitmap.width;
			spriteBuf[charOut].rows = slot->bitmap.rows;
			spriteBuf[charOut].advance = slot->advance.x >> 6;
			spriteBuf[charOut].left = slot->bitmap_left;
			spriteBuf[charOut].top = slot->bitmap_top;
			
			// Set pixel data
			memcpy(spriteBuf[charOut].data, slot->bitmap.buffer, slot->bitmap.width * slot->bitmap.rows);
		}
	}
}
