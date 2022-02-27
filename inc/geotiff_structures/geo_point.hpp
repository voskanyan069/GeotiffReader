#ifndef __GEOTIFF_STRUCTURES_GEO_POINT_HPP__
#define __GEOTIFF_STRUCTURES_GEO_POINT_HPP__

#include <cmath>
#include <string>
#include <sstream>
#include <iostream>

struct GeoPoint
{
public:
	GeoPoint();
	GeoPoint(const float& lat, const float& lon);
	void SetLatitude(const float& lat);
	void SetLongitude(const float& lon);
	float Latitude() const;
	float Longitude() const;
	float LatitudeName() const;
	float LongitudeName() const;
	std::string String();
	GeoPoint* Floor();
	~GeoPoint();

	friend bool operator<(const GeoPoint &l, const GeoPoint &r);
	friend bool operator>(const GeoPoint &l, const GeoPoint &r);

private:
	float m_latitude;
	float m_longitude;
};

#endif // __GEOTIFF_STRUCTURES_GEO_POINT_HPP__
