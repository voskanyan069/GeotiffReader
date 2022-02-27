#ifndef __GEOTIFF_STRUCTURES_GEO_PIXEL_HPP__
#define __GEOTIFF_STRUCTURES_GEO_PIXEL_HPP__

struct Pixel
{
public:
	int row;
	int col;

	Pixel()
	{
	}

	Pixel(int r, int c) : row(r), col(c)
	{
	}
};

#endif // __GEOTIFF_STRUCTURES_GEO_PIXEL_HPP__
