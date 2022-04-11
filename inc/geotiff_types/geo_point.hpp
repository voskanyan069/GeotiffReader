#ifndef __GEOTIFF_TYPES_GEO_POINT_HPP__
#define __GEOTIFF_TYPES_GEO_POINT_HPP__

#include <string>

struct GeoPoint
{
public:
	GeoPoint(const double lat, const double lon);

	double latitude() const;
	double longitude() const;
	int latitude_name() const;
	int longitude_name() const;
	std::string to_string() const;
	GeoPoint* floor() const;

	friend bool operator<(const GeoPoint& l, const GeoPoint& r);
	friend bool operator>(const GeoPoint& l, const GeoPoint& r);

private:
	const double m_latitude;
	const double m_longitude;
};

#endif // __GEOTIFF_TYPES_GEO_POINT_HPP__
