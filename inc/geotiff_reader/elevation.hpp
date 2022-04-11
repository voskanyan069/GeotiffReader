#ifndef __GEOTIFF_READER_ELEVATION_HPP__
#define __GEOTIFF_READER_ELEVATION_HPP__

#include <string>

class GeoPoint;
class GeoPixel;
class PixelSize;
class ImageSize;
class GeotiffReader;
typedef float** PixelsMatrix;

class DigitalElevationMgr
{
public:
	static DigitalElevationMgr& instance();

	DigitalElevationMgr(const DigitalElevationMgr&) = delete;
	DigitalElevationMgr& operator=(const DigitalElevationMgr&) = delete;
	~DigitalElevationMgr();

	void read(const std::string& filename);
	int get_elevation(const GeoPoint* point);
	bool is_point_exists(const GeoPoint* point);
	bool is_points_valid(const GeoPoint** points);
	void get_filename(std::string& filename, const GeoPoint* point);
	void get_filename(std::string& filename, const GeoPoint** points);

private:
	DigitalElevationMgr();

	void calculate_filename(std::string& filename, const GeoPoint* point);
	int pixel2elevation(const GeoPixel* pixel);
	void calculate_pixel(const GeoPoint* point, GeoPixel& pixel);
	void calculate_corner(const double x, const double y);

private:
	std::string m_last_filename;
	GeoPoint* m_min_point;
	GeoPoint* m_max_point;
	GeoPoint* m_area_corner;
	PixelsMatrix m_pixels;
	ImageSize* m_image_size;
	PixelSize* m_pixel_size;
	GeotiffReader* m_reader;
};

#endif // __GEOTIFF_READER_ELEVATION_HPP__
