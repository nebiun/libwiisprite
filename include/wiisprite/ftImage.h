#pragma once

#include "rgbaImage.h"

#include <set>
using std::set;

class ftImage : public rgbaImage {
public:

	class position {
	public:
		position(unsigned int x,unsigned int y);
		unsigned int x,y;
		
		bool operator <(const position &p2) const;
	};

	ftImage(const unsigned int width, const unsigned int height);
	~ftImage(void);

	void setSize(int s);
	void setColor(Color c);
	void setColor(u8 r, u8 g, u8 b);
	void setFont(const u8 *fontbuf, const u32 fontsize);
	void printf(const char *fmt, ...);
	void clear(const pixel &p);
	void clear();
	void reset();
	void operator~(void);
	void flush(void);

private:
	int _width, _height;
	//used by flood fill
	void tryToAdd(set<position> &open,bool *closed,const pixel &p,signed int xOffset,signed int yOffset,const position &pos);
};
