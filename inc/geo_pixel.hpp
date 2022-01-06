#ifndef __GEO_PIXEL_HPP__
#define __GEO_PIXEL_HPP__

struct Pixel
{
public:
	int row;
	int col;

	Pixel()
	{
	}

	Pixel(int r, int c): row(r), col(c)
	{
	}
};

#endif // __GEO_PIXEL_HPP__
