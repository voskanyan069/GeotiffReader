#ifndef __PIXEL_SIZE_HPP__
#define __PIXEL_SIZE_HPP__

struct PixelSize
{
	double x;
	double y;

	PixelSize(double x_, double y_) : x(x_), y(-1 * y_)
	{
	}
};

#endif // __PIXEL_SIZE_HPP__
