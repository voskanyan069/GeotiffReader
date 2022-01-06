#ifndef __GEO_POINT_HPP__
#define __GEO_POINT_HPP__

#include <cmath>

struct GeoPoint
{
public:
	GeoPoint();
	GeoPoint(float lat, float lon);
	void set_latitude(float lat);
	void set_longitude(float lon);
	float latitude();
	float longitude();
	float latitude_name();
	float longitude_name();
	~GeoPoint();

private:
	float lat_;
	float lon_;
};

#endif // __GEO_POINT_HPP__
