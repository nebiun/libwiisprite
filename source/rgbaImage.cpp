#include <wiisprite.h>

rgbaImage::rgbaImage(const unsigned int width, const unsigned int height) {
	_InitializeImage(width,height);
	_RGBApixels=new pixel[width*height];
};

rgbaImage::~rgbaImage(void) {
	delete[] _RGBApixels;
};

rgbaImage::pixel &rgbaImage::getpixel(const unsigned int x, const unsigned int y) {
	return _RGBApixels[x+y*GetWidth()];
};

void rgbaImage::pixel::operator ~() {
	r^=256;
	g^=256;
	b^=256;
};

bool rgbaImage::pixel::operator !=(const pixel &p2) {
	return r!=p2.r || g!=p2.g || b!=p2.b;
};

bool rgbaImage::pixel::operator ==(const pixel &p2) {
	return r==p2.r && g==p2.g && b==p2.b;
};

rgbaImage::pixel::pixel() : r(255), g(0), b(255), a(0) {};//fricking pink to notice undefined data

rgbaImage::pixel::pixel(const u8 r_p, const u8 g_p, const u8 b_p) :
	r(r_p), g(g_p), b(b_p), a(255) {
};

rgbaImage::pixel::pixel(const u8 r_p, const u8 g_p, const u8 b_p, const u8 a_p) :
	r(r_p), g(g_p), b(b_p), a(a_p) {
};

void rgbaImage::_Flush() {
	_convertPixels();
	Image::_Flush();
}

void rgbaImage::_convertPixels() {

	const unsigned int h=GetHeight(),w=GetWidth();
	//const unsigned int halftilesize=tilesize/2;

	//code shamelessly stolen from the original library

	u8 *d=_pixels;

	for (unsigned int iy = 0; iy < h; iy += tilesize) {
		for (unsigned int ix = 0; ix < w; ix += tilesize) {
			for(unsigned int iy2 = 0;iy2 <tilesize;iy2++) {
				for(unsigned int ix2 = 0;ix2 <tilesize;ix2++) {
					pixel &p=_RGBApixels[(iy+iy2)*w+(ix+ix2)];
					*d++=p.a;
					*d++=p.r;
				}
			}
			for(unsigned int iy2 = 0;iy2 <tilesize;iy2++) {
				for(unsigned int ix2 = 0;ix2 <tilesize;ix2++) {
					pixel &p=_RGBApixels[(iy+iy2)*w+(ix+ix2)];
					*d++=p.g;
					*d++=p.b;
				}
			}
		}
	}
};
