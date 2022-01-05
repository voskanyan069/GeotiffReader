#ifndef _PIXEL_SIZE_HPP
#define _PIXEL_SIZE_HPP

struct PixelSize
{
	double x;
	double y;

	PixelSize(double x_, double y_) : x(x_), y(-1 * y_)
	{
	}
};

#endif // _PIXEL_SIZE_HPP
