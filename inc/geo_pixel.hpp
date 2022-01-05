#ifndef _GEO_PIXEL_HPP
#define _GEO_PIXEL_HPP

struct Pixel
{
	int row;
	int col;

	Pixel()
	{
	}

	Pixel(int r, int c): row(r), col(c)
	{
	}
};

#endif // _GEO_PIXEL_HPP
