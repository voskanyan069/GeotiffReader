#ifndef __GEOTIFF_TYPES_PIXEL_SIZE_HPP__
#define __GEOTIFF_TYPES_PIXEL_SIZE_HPP__

struct PixelSize
{
public:
	PixelSize(const double x, const double y);

	double x() const;
	double y() const;

private:
	const double m_x;
	const double m_y;
};

#endif // __GEOTIFF_TYPES_PIXEL_SIZE_HPP__
