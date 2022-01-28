#ifndef __GEOTIFF_READER_ELEVATION_HPP__
#define __GEOTIFF_READER_ELEVATION_HPP__

#include <cmath>
#include <iostream>

#include "geotiff_reader/reader.hpp"
#include "geotiff_structures/geo_pixel.hpp"
#include "geotiff_structures/geo_point.hpp"
#include "geotiff_structures/pixel_size.hpp"
#include "geotiff_structures/image_size.hpp"

class DigitalElevation
{
public:
	DigitalElevation();
	int get_elevation(GeoPoint *point);
	bool is_point_exist(GeoPoint *point);
	bool is_valid_points(GeoPoint **points);
	void read_file(std::string filename);
	std::string get_filename(GeoPoint *point);
	std::string get_filename(GeoPoint **points);
	~DigitalElevation();

private:
	PositionsMatrix data;
	PixelSize *geotransform;
	ImageSize *image_size;
	GeoPoint *area_corner;
	GeoPoint *min_point;
	GeoPoint *max_point;

	std::string calculate_filename(GeoPoint *point);
	int elevation_from_pixel(Pixel *pixel);
	Pixel *calculate_pixel(GeoPoint *point);
	void calculate_lr_corner(double x, double y);
};

#endif // __GEOTIFF_READER_ELEVATION_HPP__
