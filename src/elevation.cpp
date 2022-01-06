#include "elevation.hpp"

DigitalElevation::DigitalElevation()
{
}

std::string DigitalElevation::calculate_filename(GeoPoint *point)
{
    //char fname[64];
    char vertical_hemisphere = 'N';
    char horizontal_hemisphere = 'E';
	if (point->latitude() < 0) vertical_hemisphere = 'S';
	if (point->longitude() < 0) horizontal_hemisphere = 'W';
	//sprintf(fname, "/home/user/projects/elevation_map/resources/geotiff/%c%02.0f%c%03.0f.tif",
	//		vertical_hemisphere, point->latitude_name(),
	//		horizontal_hemisphere, point->longitude_name());
	char* fname = "/home/user/projects/elevation_map/web_server/backend/OUT.tif";
	return fname;
}

void DigitalElevation::read_file(std::string filename)
{
	Geotiff* file = new Geotiff(filename.c_str());
	data = file->GetRasterBand(1);
	double* gt = file->GetGeoTransform();
	int* dm = file->GetDimensions();
	geotransform = new PixelSize(gt[1], gt[5]);
	image_size = new ImageSize(dm[0], dm[1]);
	calculate_lr_corner(gt[0], gt[3]);
}

void DigitalElevation::calculate_lr_corner(double x, double y)
{
	double lat = y - (image_size->height * geotransform->y);
	double lon = x + (image_size->width * geotransform->x);
	area_corner = new GeoPoint(lat, lon);
}

int DigitalElevation::get_elevation(GeoPoint* point)
{
	Pixel* pixel = calculate_pixel(point);
	int elev = elevation_from_pixel(pixel);
	return elev;
}

Pixel* DigitalElevation::calculate_pixel(GeoPoint* point)
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
	Pixel* p = new Pixel(row, col);
	return p;
}

int DigitalElevation::elevation_from_pixel(Pixel* pixel)
{
	return data[pixel->row][pixel->col];
}

DigitalElevation::~DigitalElevation()
{
}
