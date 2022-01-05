#ifndef _ELEVATION_HPP
#define _ELEVATION_HPP

#include <cmath>
#include <iostream>
#include "geotiff.hpp"
#include "geo_pixel.hpp"
#include "geo_point.hpp"
#include "pixel_size.hpp"
#include "image_size.hpp"

class DigitalElevation
{
public:
	DigitalElevation();
	int get_elevation(GeoPoint* point);
	void read_file(std::string filename);
	std::string calculate_filename(GeoPoint* point);
	~DigitalElevation();

private:
	PositionsMatrix data;
	PixelSize* geotransform;
	GeoPoint* area_corner;
	ImageSize* image_size;

	int elevation_from_pixel(Pixel* pixel);
	Pixel* calculate_pixel(GeoPoint* point);
	void calculate_lr_corner(double x, double y);
};

#endif // _ELEVATION_HPP
