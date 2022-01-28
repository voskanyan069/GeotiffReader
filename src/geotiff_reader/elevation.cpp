#include "geotiff_reader/elevation.hpp"

DigitalElevation::DigitalElevation()
{
}

std::string DigitalElevation::get_filename(GeoPoint *point)
{
	std::string filename = calculate_filename(point);
	min_point = max_point = point->as_int();
	return filename;
}

std::string DigitalElevation::get_filename(GeoPoint **points)
{
	std::string filename = calculate_filename(points[0]) + "_";
	filename += calculate_filename(points[1]) + ".tif";
	min_point = points[0]->as_int();
	max_point = points[1]->as_int();
	return filename;
}

void DigitalElevation::read_file(std::string filename)
{
	GeotiffReader *file = new GeotiffReader(filename.c_str());
	data = file->get_raster_band(1);
	double *gt = file->get_geotransform();
	int *dm = file->get_dimensions();
	geotransform = new PixelSize(gt[1], gt[5]);
	image_size = new ImageSize(dm[0], dm[1]);
	calculate_lr_corner(gt[0], gt[3]);
}

bool DigitalElevation::is_valid_points(GeoPoint **points)
{
	GeoPoint *sw = points[0];
	GeoPoint *ne = points[1];
	bool lat = sw->latitude_name() > ne->latitude_name();
	bool lon = sw->longitude_name() > ne->longitude_name();
	return !(lat || lon);
}

bool DigitalElevation::is_point_exist(GeoPoint *point)
{
	return !(*point > *max_point || *point < *min_point);
}

void DigitalElevation::calculate_lr_corner(double x, double y)
{
	double lat = y - (image_size->height  *geotransform->y);
	double lon = x + (image_size->width  *geotransform->x);
	area_corner = new GeoPoint(lat, lon);
}

int DigitalElevation::get_elevation(GeoPoint *point)
{
	Pixel *pixel = calculate_pixel(point);
	int elev = elevation_from_pixel(pixel);
	return elev;
}

Pixel *DigitalElevation::calculate_pixel(GeoPoint *point)
{
	double lat = point->latitude();
	double lon = point->longitude();
	int w = image_size->width;
	int h = image_size->height;
	double x = area_corner->latitude();
	double y = area_corner->longitude();
	double x_pixel = geotransform->x;
	double y_pixel = geotransform->y;
	int row = w - (double)((lat-x) / x_pixel);
	int col = h - (double)((y-lon) / y_pixel);
	Pixel *p = new Pixel(row, col);
	return p;
}

int DigitalElevation::elevation_from_pixel(Pixel *pixel)
{
	return data[pixel->row][pixel->col];
}

std::string DigitalElevation::calculate_filename(GeoPoint *point)
{
    char fname[8];
    char vertical_hemisphere = 'N';
    char horizontal_hemisphere = 'E';
	if (point->latitude() < 0) vertical_hemisphere = 'S';
	if (point->longitude() < 0) horizontal_hemisphere = 'W';
	sprintf(fname, "%c%02.0f%c%03.0f",
			vertical_hemisphere, point->latitude_name(),
			horizontal_hemisphere, point->longitude_name());
	return fname;
}

DigitalElevation::~DigitalElevation()
{
}
