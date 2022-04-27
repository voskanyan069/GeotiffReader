#ifndef __GEOTIFF_READER_ELEVATION_HPP__
#define __GEOTIFF_READER_ELEVATION_HPP__

#include <string>

class GeoPoint;
class GeoPixel;
class GeotiffReader;

class DigitalElevationMgr
{
public:
	static DigitalElevationMgr& instance();

	DigitalElevationMgr(const DigitalElevationMgr&) = delete;
	DigitalElevationMgr& operator=(const DigitalElevationMgr&) = delete;
	~DigitalElevationMgr();

	void read(std::string& filename);
	int get_elevation(const GeoPoint* point);
	bool is_point_exists(const GeoPoint* point);
	bool is_points_valid(const GeoPoint** points);
	void get_filename(std::string& filename, const GeoPoint* point);
	void get_filename(std::string& filename, const GeoPoint** points);

private:
	DigitalElevationMgr();

	void calculate_filename(std::string& filename, const GeoPoint* point);
	int pixel2elevation(const GeoPixel& pixel);
	void calculate_pixel(const GeoPoint* point, GeoPixel& pixel);

private:
	std::string m_last_filename;
	GeoPoint* m_min_point;
	GeoPoint* m_max_point;
	GeotiffReader* m_reader;
};

#endif // __GEOTIFF_READER_ELEVATION_HPP__
