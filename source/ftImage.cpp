#include <malloc.h>
#include <wiisprite.h>

ftImage::ftImage(const unsigned int width, const unsigned int height) : rgbaImage(width,height) {
	clear();
	setColor(255,255,255);
	_width = width;
	_height = height;
}

ftImage::~ftImage(void) {
}

void ftImage::flush(void) {
	_Flush();
};

ftImage::position::position(unsigned int x_p,unsigned int y_p) : x(x_p), y(y_p) {};

bool ftImage::position::operator <(const ftImage::position &p2) const {
	if(y==p2.y) {
		return x<p2.x;
	} else {
		return y<p2.y;
	}
};

void ftImage::setSize(int s) {
	font.setSize(s);
}

void ftImage::setColor(Color c) {
	font.setColor(c);
}

void ftImage::setColor(u8 r, u8 g, u8 b) {
	font.setColor(r, g, b);
}

void ftImage::setFont(const u8 *fontbuf, const u32 fontsize) {
	font.setFont(fontbuf, fontsize);
}

void ftImage::operator ~(void) {
	for(unsigned int iy=0;GetHeight()>iy;iy++) {
		for(unsigned int ix=0;GetWidth()>ix;ix++) {
			~getpixel(ix,iy);
		}
	}
}

void ftImage::clear(const pixel &p) {
	for(unsigned int iy=0;GetHeight()>iy;iy++) {
		for(unsigned int ix=0;GetWidth()>ix;ix++) {
			getpixel(ix,iy)=p;
		}
	}
}

void ftImage::clear() {
	for(unsigned int iy=0;GetHeight()>iy;iy++) {
		for(unsigned int ix=0;GetWidth()>ix;ix++) {
			getpixel(ix,iy)=rgbaImage::pixel(0, 0, 0, 0);
		}
	}
}

void ftImage::reset()
{
	font.reset();
}

void ftImage::printf(const char *fmt, ...)
{
	// Need to make room for the sprintf'd text
	char *out = (char *)memalign(32, 1024);
	
	// Build using sprintf
	va_list marker;
	va_start(marker,fmt);
	vsprintf(out,fmt,marker);
	va_end(marker);
	
	// Set up buffer
	// We blit directly into GX image buffer
	font.setBuffer((u8 *)_RGBApixels, _width, _height);
	
	// Call rendering engine
	font.renderSimple(out);
	
	free(out);
};

