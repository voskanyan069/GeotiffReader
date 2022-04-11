#ifndef __GEOTIFF_TYPES_GEO_PIXEL_HPP__
#define __GEOTIFF_TYPES_GEO_PIXEL_HPP__

struct GeoPixel
{
public:
	GeoPixel();
	GeoPixel(const int r, const int c);

	int row() const;
	int col() const;

private:
	int m_row;
	int m_col;
};

#endif // __GEOTIFF_TYPES_GEO_PIXEL_HPP__
